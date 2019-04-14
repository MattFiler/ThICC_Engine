//--------------------------------------------------------------------------------------
// File: Game.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Game.h"

#include "FindMedia.h"
#include "ReadData.h"
#include "SDKMesh.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

namespace
{
	const XMVECTORF32 c_Gray = { 0.215861f, 0.215861f, 0.215861f, 1.f };
	const XMVECTORF32 c_CornflowerBlue = { 0.127438f, 0.300544f, 0.846873f, 1.f };
}

Game::Game() noexcept(false) :
	m_gridScale(10.f),
	m_fov(XM_PI / 4.f),
	m_zoom(1.f),
	m_distance(10.f),
	m_farPlane(10000.f),
	m_sensitivity(1.f),
	m_gridDivs(20),
	m_ibl(0),
	m_showHud(true),
	m_showCross(true),
	m_showGrid(false),
	m_usingGamepad(false),
	m_wireframe(false),
	m_ccw(true),
	m_reloadModel(false),
	m_lhcoords(true),
	m_fpscamera(false),
	m_toneMapMode(ToneMapPostProcess::Reinhard),
	m_selectFile(0),
	m_firstFile(0)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>(DXGI_FORMAT_R10G10B10A2_UNORM, DXGI_FORMAT_D32_FLOAT, 2,
		D3D_FEATURE_LEVEL_11_0, DX::DeviceResources::c_EnableHDR);
	m_deviceResources->RegisterDeviceNotify(this);

	m_hdrScene = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R16G16B16A16_FLOAT);

	m_clearColor = Colors::Black.v;
	m_uiColor = Colors::Yellow;

	*m_szModelName = 0;
	*m_szStatus = 0;
	*m_szError = 0;
}

Game::~Game()
{
	if (m_deviceResources)
	{
		m_deviceResources->WaitForGpu();
	}
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_gamepad = std::make_unique<GamePad>();
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();

	m_deviceResources->SetWindow(window, width, height);
	m_mouse->SetWindow(window);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	if (m_reloadModel)
		LoadModel();

	float elapsedTime = float(timer.GetElapsedSeconds());

	float handed = (m_lhcoords) ? 1.f : -1.f;

	auto gpad = m_gamepad->GetState(0);
	if (gpad.IsConnected())
	{
		m_usingGamepad = true;

		m_gamepadButtonTracker.Update(gpad);

		if (!m_fileNames.empty())
		{
			if (m_gamepadButtonTracker.dpadUp == GamePad::ButtonStateTracker::PRESSED)
			{
				--m_selectFile;
				if (m_selectFile < 0)
					m_selectFile = int(m_fileNames.size()) - 1;
			}
			else if (m_gamepadButtonTracker.dpadDown == GamePad::ButtonStateTracker::PRESSED)
			{
				++m_selectFile;
				if (m_selectFile >= int(m_fileNames.size()))
					m_selectFile = 0;
			}
			else if (gpad.IsAPressed())
			{
				swprintf_s(m_szModelName, L"D:\\%ls", m_fileNames[m_selectFile].c_str());
				m_selectFile = m_firstFile = 0;
				m_fileNames.clear();
				LoadModel();
			}
			else if (gpad.IsBPressed())
			{
				m_selectFile = m_firstFile = 0;
				m_fileNames.clear();
			}
		}
		else
		{
			// Translate camera
			Vector3 move;

			if (m_fpscamera)
			{
				move.x = gpad.thumbSticks.leftX;
				move.z = gpad.thumbSticks.leftY * handed;
			}
			else
			{
				m_zoom -= gpad.thumbSticks.leftY * elapsedTime * m_sensitivity;
				m_zoom = std::max(m_zoom, 0.01f);
			}

			if (gpad.IsDPadUpPressed())
			{
				move.y += 1.f;
			}
			else if (gpad.IsDPadDownPressed())
			{
				move.y -= 1.f;
			}

			if (gpad.IsDPadLeftPressed())
			{
				move.x -= 1.f;
			}
			else if (gpad.IsDPadRightPressed())
			{
				move.x += 1.f;
			}

			Matrix im;
			m_view.Invert(im);
			move = Vector3::TransformNormal(move, im);

			// Rotate camera
			if (m_fpscamera)
			{
				Vector3 spin(gpad.thumbSticks.rightX, gpad.thumbSticks.rightY, 0);
				spin *= elapsedTime * m_sensitivity;

				Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::Right, spin.y * handed);
				q *= Quaternion::CreateFromAxisAngle(Vector3::Up, -spin.x * handed);
				q.Normalize();

				RotateView(q);
			}
			else
			{
				Vector3 orbit(gpad.thumbSticks.rightX, gpad.thumbSticks.rightY, gpad.thumbSticks.leftX);
				orbit *= elapsedTime * m_sensitivity;

				m_cameraRot *= Quaternion::CreateFromAxisAngle(im.Right(), orbit.y * handed);
				m_cameraRot *= Quaternion::CreateFromAxisAngle(im.Up(), -orbit.x * handed);
				m_cameraRot *= Quaternion::CreateFromAxisAngle(im.Forward(), orbit.z);
				m_cameraRot.Normalize();
			}

			m_cameraFocus += move * m_distance * elapsedTime * m_sensitivity;

			// FOV camera
			if (gpad.triggers.right > 0 || gpad.triggers.left > 0)
			{
				m_fov += (gpad.triggers.right - gpad.triggers.left) * elapsedTime * m_sensitivity;
				m_fov = std::min(XM_PI / 2.f, std::max(m_fov, XM_PI / 10.f));

				CreateProjection();
			}

			// Other controls
			if (m_gamepadButtonTracker.leftShoulder == GamePad::ButtonStateTracker::PRESSED
				&& m_gamepadButtonTracker.rightShoulder == GamePad::ButtonStateTracker::PRESSED)
			{
				m_sensitivity = 1.f;
			}
			else
			{
				if (gpad.IsRightShoulderPressed())
				{
					m_sensitivity += 0.01f;
					if (m_sensitivity > 10.f)
						m_sensitivity = 10.f;
				}
				else if (gpad.IsLeftShoulderPressed())
				{
					m_sensitivity -= 0.01f;
					if (m_sensitivity < 0.01f)
						m_sensitivity = 0.01f;
				}
			}

			if (gpad.IsViewPressed())
			{
				PostMessage(m_deviceResources->GetWindowHandle(), WM_USER, 0, 0);
			}

			if (m_gamepadButtonTracker.menu == GamePad::ButtonStateTracker::PRESSED)
			{
				CycleToneMapOperator();
			}

			if (m_gamepadButtonTracker.b == GamePad::ButtonStateTracker::PRESSED)
			{
				int value = ((int)m_ccw << 1) | ((int)m_wireframe);

				value = value + 1;
				if (value >= 3) value = 0;

				m_wireframe = (value & 0x1) != 0;
				m_ccw = (value & 0x2) != 0;
			}

			if (m_gamepadButtonTracker.x == GamePad::ButtonStateTracker::PRESSED)
			{
				int value = ((int)m_showGrid << 2) | ((int)m_showHud << 1) | ((int)m_showCross);

				value = (value + 1) & 0x7;

				m_showCross = (value & 0x1) != 0;
				m_showHud = (value & 0x2) != 0;
				m_showGrid = (value & 0x4) != 0;
			}

			if (m_gamepadButtonTracker.y == GamePad::ButtonStateTracker::PRESSED)
			{
				CycleBackgroundColor();
			}

			if (m_gamepadButtonTracker.a == GamePad::ButtonStateTracker::PRESSED)
			{
				m_fpscamera = !m_fpscamera;
			}

			if (m_gamepadButtonTracker.leftStick == GamePad::ButtonStateTracker::PRESSED)
			{
				CameraHome();
				m_modelRot = Quaternion::Identity;
			}

			if (!m_fpscamera && m_gamepadButtonTracker.rightStick == GamePad::ButtonStateTracker::PRESSED)
			{
				m_cameraRot = m_modelRot = Quaternion::Identity;
			}

			// TODO - m_ibl
		}
	}
	else
	{
		m_usingGamepad = false;

		m_gamepadButtonTracker.Reset();

		auto kb = m_keyboard->GetState();

		// Camera movement
		Vector3 move = Vector3::Zero;

		float scale = m_distance;
		if (kb.LeftShift || kb.RightShift)
			scale *= 0.5f;

		if (kb.Up)
			move.y += scale;

		if (kb.Down)
			move.y -= scale;

		if (kb.Right || kb.D)
			move.x += scale;

		if (kb.Left || kb.A)
			move.x -= scale;

		if (kb.PageUp || kb.W)
			move.z += scale * handed;

		if (kb.PageDown || kb.S)
			move.z -= scale * handed;

		if (kb.Q || kb.E)
		{
			float flip = (kb.Q) ? 1.f : -1.f;
			Quaternion q = Quaternion::CreateFromAxisAngle(Vector3::Up, handed * elapsedTime * flip);

			RotateView(q);
		}

		Matrix im;
		m_view.Invert(im);
		move = Vector3::TransformNormal(move, im);

		m_cameraFocus += move * elapsedTime;

		// FOV camera
		if (kb.OemOpenBrackets || kb.OemCloseBrackets)
		{
			if (kb.OemOpenBrackets)
				m_fov += elapsedTime;
			else if (kb.OemCloseBrackets)
				m_fov -= elapsedTime;

			m_fov = std::min(XM_PI / 2.f, std::max(m_fov, XM_PI / 10.f));

			CreateProjection();
		}

		// Other keyboard controls
		if (kb.Home)
		{
			CameraHome();
		}

		if (kb.End)
		{
			m_modelRot = Quaternion::Identity;
		}

		if (m_showGrid)
		{
			if (kb.OemPlus)
			{
				m_gridScale += 2.f * elapsedTime;
			}
			else if (kb.OemMinus)
			{
				m_gridScale -= 2.f * elapsedTime;
				if (m_gridScale < 1.f)
					m_gridScale = 1.f;
			}
		}

		m_keyboardTracker.Update(kb);

		if (m_keyboardTracker.pressed.J)
			m_showCross = !m_showCross;

		if (m_keyboardTracker.pressed.G)
			m_showGrid = !m_showGrid;

		if (m_keyboardTracker.pressed.R)
			m_wireframe = !m_wireframe;

		if (m_keyboardTracker.pressed.B && !m_wireframe)
			m_ccw = !m_ccw;

		if (m_keyboardTracker.pressed.H)
			m_showHud = !m_showHud;

		if (m_keyboardTracker.pressed.C)
			CycleBackgroundColor();

		if (m_keyboardTracker.pressed.T)
			CycleToneMapOperator();

		if (m_keyboardTracker.pressed.O)
		{
			PostMessage(m_deviceResources->GetWindowHandle(), WM_USER, 0, 0);
		}

		if (m_keyboardTracker.IsKeyPressed(Keyboard::Enter) && !kb.LeftAlt && !kb.RightAlt)
		{
			++m_ibl;
			if (m_ibl >= s_nIBL)
			{
				m_ibl = 0;
			}
		}
		else if (m_keyboardTracker.IsKeyPressed(Keyboard::Back))
		{
			if (!m_ibl)
				m_ibl = s_nIBL - 1;
			else
				--m_ibl;
		}

		// Mouse controls
		auto mouse = m_mouse->GetState();

		m_mouseButtonTracker.Update(mouse);

		if (mouse.positionMode == Mouse::MODE_RELATIVE)
		{
			// Translate camera
			Vector3 delta;
			if (kb.LeftShift || kb.RightShift)
			{
				delta = Vector3(0.f, 0.f, -float(mouse.y) * handed) * m_distance * elapsedTime;
			}
			else
			{
				delta = Vector3(-float(mouse.x), float(mouse.y), 0.f) * m_distance * elapsedTime;
			}

			delta = Vector3::TransformNormal(delta, im);

			m_cameraFocus += delta * elapsedTime;
		}
		else if (m_ballModel.IsDragging())
		{
			// Rotate model
			m_ballModel.OnMove(mouse.x, mouse.y);
			m_modelRot = m_ballModel.GetQuat();
		}
		else if (m_ballCamera.IsDragging())
		{
			// Rotate camera
			m_ballCamera.OnMove(mouse.x, mouse.y);
			Quaternion q = m_ballCamera.GetQuat();
			q.Inverse(m_cameraRot);
		}
		else
		{
			// Zoom with scroll wheel
			m_zoom = 1.f + float(mouse.scrollWheelValue) / float(120 * 10);
			m_zoom = std::max(m_zoom, 0.01f);
		}

		if (!m_ballModel.IsDragging() && !m_ballCamera.IsDragging())
		{
			if (m_mouseButtonTracker.rightButton == Mouse::ButtonStateTracker::PRESSED)
				m_mouse->SetMode(Mouse::MODE_RELATIVE);
			else if (m_mouseButtonTracker.rightButton == Mouse::ButtonStateTracker::RELEASED)
				m_mouse->SetMode(Mouse::MODE_ABSOLUTE);

			if (m_mouseButtonTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
			{
				if (kb.LeftShift || kb.RightShift)
				{
					m_ballModel.OnBegin(mouse.x, mouse.y);
				}
				else
				{
					m_ballCamera.OnBegin(mouse.x, mouse.y);
				}
			}
		}
		else if (m_mouseButtonTracker.leftButton == Mouse::ButtonStateTracker::RELEASED)
		{
			m_ballCamera.OnEnd();
			m_ballModel.OnEnd();
		}
	}

	// Update camera
	Vector3 dir = Vector3::Transform((m_lhcoords) ? Vector3::Forward : Vector3::Backward, m_cameraRot);
	Vector3 up = Vector3::Transform(Vector3::Up, m_cameraRot);

	m_lastCameraPos = m_cameraFocus + (m_distance * m_zoom) * dir;

	if (m_lhcoords)
	{
		m_view = XMMatrixLookAtLH(m_lastCameraPos, m_cameraFocus, up);
	}
	else
	{
		m_view = Matrix::CreateLookAt(m_lastCameraPos, m_cameraFocus, up);
	}

	m_world = Matrix::CreateFromQuaternion(m_modelRot);
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	// Prepare the command list to render a new frame.
	m_deviceResources->Prepare();

	auto commandList = m_deviceResources->GetCommandList();
	m_hdrScene->BeginScene(commandList);

	Clear();

	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	RECT size = m_deviceResources->GetOutputSize();

	if (!m_fileNames.empty())
	{
		m_spriteBatch->Begin(commandList);

		RECT rct = Viewport::ComputeTitleSafeArea(size.right, size.bottom);

		float spacing = m_fontComic->GetLineSpacing();

		int maxl = static_cast<int>(float(rct.bottom - rct.top + spacing) / spacing);

		if (m_selectFile < m_firstFile)
			m_firstFile = m_selectFile;

		if (m_selectFile > (m_firstFile + maxl))
			m_firstFile = m_selectFile;

		float y = float(rct.top);
		for (int j = m_firstFile; j < int(m_fileNames.size()); ++j)
		{
			XMVECTOR hicolor = m_uiColor;
			m_fontComic->DrawString(m_spriteBatch.get(), m_fileNames[j].c_str(), XMFLOAT2(float(rct.left), y), (m_selectFile == j) ? hicolor : c_Gray);
			y += spacing;
		}

		m_spriteBatch->End();
	}
	else
	{
		if (m_showGrid)
		{
			DrawGrid(commandList);
		}

		if (m_showCross)
		{
			DrawCross(commandList);
		}

		if (!m_model)
		{
			m_spriteBatch->Begin(commandList);

			if (*m_szError)
			{
				m_fontComic->DrawString(m_spriteBatch.get(), m_szError, XMFLOAT2(100, 100), Colors::Red);
			}
			else
			{
				m_fontComic->DrawString(m_spriteBatch.get(), L"No model is loaded\n", XMFLOAT2(100, 100), Colors::Red);
			}

			m_spriteBatch->End();
		}
		else
		{
			{
				auto radianceTex = m_resourceDescriptors->GetGpuHandle(Descriptors::RadianceIBL1 + m_ibl);

				auto diffuseDesc = m_radianceIBL[0]->GetDesc();

				auto irradianceTex = m_resourceDescriptors->GetGpuHandle(Descriptors::IrradianceIBL1 + m_ibl);

				for (auto& it : m_modelWireframe)
				{
					auto pbr = dynamic_cast<PBREffect*>(it.get());
					if (pbr)
					{
						pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, m_states->AnisotropicClamp());
					}
				}
				for (auto& it : m_modelCounterClockwise)
				{
					auto pbr = dynamic_cast<PBREffect*>(it.get());
					if (pbr)
					{
						pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, m_states->AnisotropicClamp());
					}
				}
				for (auto& it : m_modelClockwise)
				{
					auto pbr = dynamic_cast<PBREffect*>(it.get());
					if (pbr)
					{
						pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, m_states->AnisotropicClamp());
					}
				}
			}

			if (m_wireframe)
			{
				Model::UpdateEffectMatrices(m_modelWireframe, m_world, m_view, m_proj);

				m_model->Draw(commandList, m_modelWireframe.cbegin());
			}
			else if (m_ccw)
			{
				Model::UpdateEffectMatrices(m_modelCounterClockwise, m_world, m_view, m_proj);

				m_model->Draw(commandList, m_modelCounterClockwise.cbegin());
			}
			else
			{
				Model::UpdateEffectMatrices(m_modelClockwise, m_world, m_view, m_proj);

				m_model->Draw(commandList, m_modelClockwise.cbegin());
			}

			if (*m_szStatus && m_showHud)
			{
				m_spriteBatch->Begin(commandList);

				Vector3 up = Vector3::TransformNormal(Vector3::Up, m_view);

				wchar_t szCamera[256] = {};
				swprintf_s(szCamera, L"Camera: (%8.4f,%8.4f,%8.4f) Look At: (%8.4f,%8.4f,%8.4f) Up: (%8.4f,%8.4f,%8.4f) FOV: %8.4f",
					m_lastCameraPos.x, m_lastCameraPos.y, m_lastCameraPos.z,
					m_cameraFocus.x, m_cameraFocus.y, m_cameraFocus.z,
					up.x, up.y, up.z, XMConvertToDegrees(m_fov));

				const wchar_t* mode = m_ccw ? L"Counter clockwise" : L"Clockwise";
				if (m_wireframe)
					mode = L"Wireframe";

				const wchar_t* toneMap = L"*UNKNOWN*";
				switch (m_deviceResources->GetColorSpace())
				{
				default:
					switch (m_toneMapMode)
					{
					case ToneMapPostProcess::Saturate: toneMap = L"None"; break;
					case ToneMapPostProcess::Reinhard: toneMap = L"Reinhard"; break;
					case ToneMapPostProcess::ACESFilmic: toneMap = L"ACES Filmic"; break;
					}
					break;

				case DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020:
					toneMap = L"HDR10";
					break;

				case DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709:
					toneMap = L"Linear";
					break;
				}

				wchar_t szState[128] = {};
				swprintf_s(szState, L"%ls    Tone-mapping operator: %ls", mode, toneMap);

				wchar_t szMode[64] = {};
				swprintf_s(szMode, L" %ls (Sensitivity: %8.4f)", (m_fpscamera) ? L"  FPS" : L"Orbit", m_sensitivity);

				Vector2 modeLen = m_fontConsolas->MeasureString(szMode);

				float spacing = m_fontConsolas->GetLineSpacing();

				m_fontConsolas->DrawString(m_spriteBatch.get(), m_szStatus, XMFLOAT2(0, 10), m_uiColor);
				m_fontConsolas->DrawString(m_spriteBatch.get(), szCamera, XMFLOAT2(0, 10 + spacing), m_uiColor);
				m_fontConsolas->DrawString(m_spriteBatch.get(), szState, XMFLOAT2(0, 10 + spacing * 2.f), m_uiColor);
				if (m_usingGamepad)
				{
					m_fontConsolas->DrawString(m_spriteBatch.get(), szMode, XMFLOAT2(size.right - modeLen.x, size.bottom - modeLen.y), m_uiColor);
				}

				m_spriteBatch->End();
			}
		}
	}

	m_hdrScene->EndScene(commandList);

	PIXEndEvent(commandList);

	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"ToneMap");

	auto rtvDescriptor = m_deviceResources->GetRenderTargetView();
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);

	switch (m_deviceResources->GetColorSpace())
	{
	default:
		switch (m_toneMapMode)
		{
		case ToneMapPostProcess::Reinhard:      m_toneMapReinhard->Process(commandList); break;
		case ToneMapPostProcess::ACESFilmic:    m_toneMapACESFilmic->Process(commandList); break;
		default:                                m_toneMapSaturate->Process(commandList); break;
		}
		break;

	case DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020:
		m_toneMapHDR10->Process(commandList);
		break;

	case DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709:
		m_toneMapLinear->Process(commandList);
		break;
	}

	PIXEndEvent(commandList);

	// Show the new frame.
	PIXBeginEvent(m_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");
	m_deviceResources->Present();
	m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());
	PIXEndEvent(m_deviceResources->GetCommandQueue());
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	auto commandList = m_deviceResources->GetCommandList();
	PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

	// Clear the views.
	auto rtvDescriptor = m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene);
	auto dsvDescriptor = m_deviceResources->GetDepthStencilView();

	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	commandList->ClearRenderTargetView(rtvDescriptor, m_clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto viewport = m_deviceResources->GetScreenViewport();
	auto scissorRect = m_deviceResources->GetScissorRect();
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

	PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
	m_keyboardTracker.Reset();
	m_mouseButtonTracker.Reset();
	m_gamepadButtonTracker.Reset();
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();
	m_keyboardTracker.Reset();
	m_mouseButtonTracker.Reset();
	m_gamepadButtonTracker.Reset();
}

void Game::OnWindowMoved()
{
	auto r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();
}

void Game::OnFileOpen(const wchar_t* filename)
{
	if (!filename)
		return;

	wcscpy_s(m_szModelName, filename);
	m_reloadModel = true;
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	width = 1280;
	height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();

	m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

	m_resourceDescriptors = std::make_unique<DescriptorPile>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		Descriptors::Count,
		Descriptors::Reserve);

	m_renderDescriptors = std::make_unique<DescriptorHeap>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		RTVDescriptors::RTVCount);

	m_hdrScene->SetDevice(device, m_resourceDescriptors->GetCpuHandle(Descriptors::SceneTex), m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene));

	m_states = std::make_unique<CommonStates>(device);

	m_lineBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(device);

	RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), DXGI_FORMAT_UNKNOWN);

	m_toneMapSaturate = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::Saturate, ToneMapPostProcess::SRGB);
	m_toneMapReinhard = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::Reinhard, ToneMapPostProcess::SRGB);
	m_toneMapACESFilmic = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::ACESFilmic, ToneMapPostProcess::SRGB);

	m_toneMapSaturate->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex));
	m_toneMapReinhard->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex));
	m_toneMapACESFilmic->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex));

	m_toneMapLinear = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::None, ToneMapPostProcess::Linear);
	m_toneMapLinear->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex));

	m_toneMapHDR10 = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::None, ToneMapPostProcess::ST2084);
	m_toneMapHDR10->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex));

	RenderTargetState hdrState(m_hdrScene->GetFormat(), m_deviceResources->GetDepthBufferFormat());

	{
		EffectPipelineStateDescription pd(
			&VertexPositionColor::InputLayout,
			CommonStates::Opaque,
			CommonStates::DepthRead,
			CommonStates::CullNone,
			hdrState, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

		m_lineEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, pd);
	}

	ResourceUploadBatch resourceUpload(device);

	resourceUpload.Begin();

	{
		SpriteBatchPipelineStateDescription pd(hdrState);
		m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);
	}

	static const wchar_t* s_radianceIBL[s_nIBL] =
	{
		L"Atrium_diffuseIBL.dds",
		L"Garage_diffuseIBL.dds",
		L"SunSubMixer_diffuseIBL.dds",
	};
	static const wchar_t* s_irradianceIBL[s_nIBL] =
	{
		L"Atrium_specularIBL.dds",
		L"Garage_specularIBL.dds",
		L"SunSubMixer_specularIBL.dds",
	};

	static_assert(_countof(s_radianceIBL) == _countof(s_irradianceIBL), "IBL array mismatch");

	for (size_t j = 0; j < s_nIBL; ++j)
	{
		wchar_t radiance[_MAX_PATH] = {};
		wchar_t irradiance[_MAX_PATH] = {};

		DX::FindMediaFile(radiance, _MAX_PATH, s_radianceIBL[j]);
		DX::FindMediaFile(irradiance, _MAX_PATH, s_irradianceIBL[j]);

		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, radiance, m_radianceIBL[j].ReleaseAndGetAddressOf())
		);

		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, irradiance, m_irradianceIBL[j].ReleaseAndGetAddressOf())
		);

		CreateShaderResourceView(device, m_radianceIBL[j].Get(), m_resourceDescriptors->GetCpuHandle(Descriptors::RadianceIBL1 + j), true);
		CreateShaderResourceView(device, m_irradianceIBL[j].Get(), m_resourceDescriptors->GetCpuHandle(Descriptors::IrradianceIBL1 + j), true);
	}

	auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());

	m_deviceResources->WaitForGpu();

	uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();

	auto size = m_deviceResources->GetOutputSize();

	ResourceUploadBatch resourceUpload(device);

	resourceUpload.Begin();

	wchar_t consolasFont[_MAX_PATH] = {};
	wchar_t comicFont[_MAX_PATH] = {};

	DX::FindMediaFile(consolasFont, _MAX_PATH, (size.bottom > 1200) ? L"consolas4k.spritefont" : L"consolas.spritefont");
	DX::FindMediaFile(comicFont, _MAX_PATH, (size.bottom > 1200) ? L"comic4k.spritefont" : L"comic.spritefont");

	m_fontConsolas = std::make_unique<SpriteFont>(device, resourceUpload, consolasFont,
		m_resourceDescriptors->GetCpuHandle(Descriptors::ConsolasFont),
		m_resourceDescriptors->GetGpuHandle(Descriptors::ConsolasFont));

	m_fontComic = std::make_unique<SpriteFont>(device, resourceUpload, comicFont,
		m_resourceDescriptors->GetCpuHandle(Descriptors::ComicFont),
		m_resourceDescriptors->GetGpuHandle(Descriptors::ComicFont));

	m_deviceResources->WaitForGpu();

	auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());

	uploadResourcesFinished.wait();

	auto viewport = m_deviceResources->GetScreenViewport();
	m_spriteBatch->SetViewport(viewport);

	m_hdrScene->SetWindow(size);

	m_ballCamera.SetWindow(size.right, size.bottom);
	m_ballModel.SetWindow(size.right, size.bottom);

	CreateProjection();
}

void Game::OnDeviceLost()
{
	m_spriteBatch.reset();
	m_fontConsolas.reset();
	m_fontComic.reset();

	m_fxFactory.reset();
	m_pbrFXFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_modelClockwise.clear();
	m_modelCounterClockwise.clear();
	m_modelWireframe.clear();

	m_lineEffect.reset();
	m_lineBatch.reset();

	m_states.reset();

	m_toneMapSaturate.reset();
	m_toneMapReinhard.reset();
	m_toneMapACESFilmic.reset();

	m_toneMapLinear.reset();
	m_toneMapHDR10.reset();

	m_resourceDescriptors.reset();
	m_renderDescriptors.reset();

	m_hdrScene->ReleaseDevice();

	m_graphicsMemory.reset();
}

void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion

void Game::LoadModel()
{
	m_modelClockwise.clear();
	m_modelCounterClockwise.clear();
	m_modelWireframe.clear();
	m_modelResources.reset();
	m_model.reset();
	m_fxFactory.reset();
	m_pbrFXFactory.reset();

	*m_szStatus = 0;
	*m_szError = 0;
	m_reloadModel = false;
	m_modelRot = Quaternion::Identity;

	if (!*m_szModelName)
		return;

	wchar_t drive[_MAX_DRIVE] = {};
	wchar_t path[MAX_PATH] = {};
	wchar_t ext[_MAX_EXT] = {};
	wchar_t fname[_MAX_FNAME] = {};
	_wsplitpath_s(m_szModelName, drive, _MAX_DRIVE, path, MAX_PATH, fname, _MAX_FNAME, ext, _MAX_EXT);

	bool isvbo = false;
	bool issdkmesh2 = false;
	try
	{
		if (_wcsicmp(ext, L".sdkmesh") == 0)
		{
			auto modelBin = DX::ReadData(m_szModelName);

			if (modelBin.size() >= sizeof(DXUT::SDKMESH_HEADER))
			{
				auto hdr = reinterpret_cast<const DXUT::SDKMESH_HEADER*>(modelBin.data());
				if (hdr->Version >= 200)
				{
					issdkmesh2 = true;
				}
			}

			m_model = Model::CreateFromSDKMESH(modelBin.data(), modelBin.size());
		}
		else if (_wcsicmp(ext, L".vbo") == 0)
		{
			isvbo = true;
			m_model = Model::CreateFromVBO(m_szModelName);
		}
		else
		{
			swprintf_s(m_szError, L"Unknown file type %ls", ext);
			m_model.reset();
			*m_szStatus = 0;
		}
	}
	catch (...)
	{
		swprintf_s(m_szError, L"Error loading model %ls%ls\n", fname, ext);
		m_model.reset();
		*m_szStatus = 0;
	}

	m_wireframe = false;

	if (m_model)
	{
		auto device = m_deviceResources->GetD3DDevice();

		ResourceUploadBatch resourceUpload(device);

		resourceUpload.Begin();

		m_model->LoadStaticBuffers(device, resourceUpload, true);

		m_modelResources = std::make_unique<EffectTextureFactory>(device, resourceUpload, m_resourceDescriptors->Heap());

		if (!issdkmesh2)
		{
			m_modelResources->EnableForceSRGB(true);
		}

		if (*drive || *path)
		{
			wchar_t dir[MAX_PATH] = {};
			_wmakepath_s(dir, drive, path, nullptr, nullptr);
			m_modelResources->SetDirectory(dir);
		}

		int txtOffset = Descriptors::Reserve;

		try
		{
			(void)m_model->LoadTextures(*m_modelResources, txtOffset);
		}
		catch (...)
		{
			swprintf_s(m_szError, L"Error loading textures for model %ls%ls\n", fname, ext);
			m_model.reset();
			m_modelResources.reset();
			*m_szStatus = 0;
		}

		if (m_model)
		{
			IEffectFactory *fxFactory = nullptr;
			if (issdkmesh2)
			{
				m_pbrFXFactory = std::make_unique<PBREffectFactory>(m_modelResources->Heap(), m_states->Heap());
				fxFactory = m_pbrFXFactory.get();
			}
			else
			{
				m_fxFactory = std::make_unique<EffectFactory>(m_modelResources->Heap(), m_states->Heap());
				fxFactory = m_fxFactory.get();
			}

			RenderTargetState hdrState(m_hdrScene->GetFormat(), m_deviceResources->GetDepthBufferFormat());

			if (isvbo)
			{
				EffectPipelineStateDescription pd(
					&VertexPositionNormalTexture::InputLayout,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					CommonStates::CullClockwise,
					hdrState);

				auto effect = std::make_shared<BasicEffect>(device, EffectFlags::Lighting, pd);
				effect->EnableDefaultLighting();
				m_modelClockwise.push_back(effect);

				pd.rasterizerDesc = CommonStates::CullCounterClockwise;

				effect = std::make_shared<BasicEffect>(device, EffectFlags::Lighting, pd);
				effect->EnableDefaultLighting();
				m_modelCounterClockwise.push_back(effect);

				pd.rasterizerDesc = CommonStates::Wireframe;

				effect = std::make_shared<BasicEffect>(device, EffectFlags::Lighting, pd);
				effect->EnableDefaultLighting();
				m_modelWireframe.push_back(effect);
			}
			else
			{
				EffectPipelineStateDescription pd(
					nullptr,
					CommonStates::Opaque,
					CommonStates::DepthDefault,
					CommonStates::CullClockwise,
					hdrState);

				EffectPipelineStateDescription pdAlpha(
					nullptr,
					CommonStates::AlphaBlend,
					CommonStates::DepthDefault,
					CommonStates::CullClockwise,
					hdrState);

				m_modelClockwise = m_model->CreateEffects(*fxFactory, pd, pdAlpha, txtOffset);

				pd.rasterizerDesc = CommonStates::CullCounterClockwise;
				pdAlpha.rasterizerDesc = CommonStates::CullCounterClockwise;

				m_modelCounterClockwise = m_model->CreateEffects(*fxFactory, pd, pdAlpha, txtOffset);

				pd.rasterizerDesc = CommonStates::Wireframe;
				pdAlpha.rasterizerDesc = CommonStates::Wireframe;

				m_modelWireframe = m_model->CreateEffects(*fxFactory, pd, pdAlpha, txtOffset);
			}

			// Compute mesh stats
			size_t nmeshes = 0;
			size_t nverts = 0;
			size_t nfaces = 0;
			size_t nsubsets = 0;

			std::set<void*> vbs;
			for (auto it = m_model->meshes.cbegin(); it != m_model->meshes.cend(); ++it)
			{
				for (auto mit = (*it)->opaqueMeshParts.cbegin(); mit != (*it)->opaqueMeshParts.cend(); ++mit)
				{
					++nsubsets;

					nfaces += ((*mit)->indexCount / 3);

					size_t vertexStride = (*mit)->vertexStride;

					void *mem = (*mit)->vertexBuffer.Memory();

					if ((*mit)->vertexBuffer && (vertexStride > 0) && vbs.find(mem) == vbs.end())
					{
						nverts += ((*mit)->vertexBuffer.Size() / vertexStride);
						vbs.insert(mem);
					}
				}
				++nmeshes;
			}

			if (nmeshes > 1)
			{
				swprintf_s(m_szStatus, L"Meshes: %6Iu   Verts: %6Iu   Faces: %6Iu   Subsets: %6Iu", nmeshes, nverts, nfaces, nsubsets);
			}
			else
			{
				swprintf_s(m_szStatus, L"Verts: %6Iu   Faces: %6Iu   Subsets: %6Iu", nverts, nfaces, nsubsets);
			}
		}

		auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());

		m_deviceResources->WaitForGpu();

		uploadResourcesFinished.wait();
	}

	CameraHome();
}

void Game::DrawGrid(ID3D12GraphicsCommandList *commandList)
{
	m_lineEffect->SetView(m_view);

	m_lineEffect->Apply(commandList);

	m_lineBatch->Begin(commandList);

	Vector3 xaxis(m_gridScale, 0.f, 0.f);
	Vector3 yaxis(0.f, 0.f, m_gridScale);
	Vector3 origin = Vector3::Zero;

	XMVECTOR color = m_uiColor;

	for (size_t i = 0; i <= m_gridDivs; ++i)
	{
		float fPercent = float(i) / float(m_gridDivs);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = xaxis * fPercent + origin;

		VertexPositionColor v1(scale - yaxis, color);
		VertexPositionColor v2(scale + yaxis, color);
		m_lineBatch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= m_gridDivs; i++)
	{
		float fPercent = float(i) / float(m_gridDivs);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = yaxis * fPercent + origin;

		VertexPositionColor v1(scale - xaxis, color);
		VertexPositionColor v2(scale + xaxis, color);
		m_lineBatch->DrawLine(v1, v2);
	}

	m_lineBatch->End();
}

void Game::DrawCross(ID3D12GraphicsCommandList *commandList)
{
	m_lineEffect->SetView(m_view);

	m_lineEffect->Apply(commandList);

	XMVECTOR color = m_uiColor;

	m_lineBatch->Begin(commandList);

	float cross = m_distance / 100.f;

	Vector3 xaxis(cross, 0.f, 0.f);
	Vector3 yaxis(0.f, cross, 0.f);
	Vector3 zaxis(0.f, 0.f, cross);

	VertexPositionColor v1(m_cameraFocus - xaxis, color);
	VertexPositionColor v2(m_cameraFocus + xaxis, color);

	m_lineBatch->DrawLine(v1, v2);

	VertexPositionColor v3(m_cameraFocus - yaxis, color);
	VertexPositionColor v4(m_cameraFocus + yaxis, color);

	m_lineBatch->DrawLine(v3, v4);

	VertexPositionColor v5(m_cameraFocus - zaxis, color);
	VertexPositionColor v6(m_cameraFocus + zaxis, color);

	m_lineBatch->DrawLine(v5, v6);

	m_lineBatch->End();
}

void Game::CameraHome()
{
	m_mouse->ResetScrollWheelValue();
	m_zoom = 1.f;
	m_fov = XM_PI / 4.f;
	m_cameraRot = Quaternion::Identity;
	m_ballCamera.Reset();

	if (!m_model)
	{
		m_cameraFocus = Vector3::Zero;
		m_distance = 10.f;
		m_gridScale = 1.f;
	}
	else
	{
		BoundingSphere sphere;
		BoundingBox box;

		for (auto it = m_model->meshes.cbegin(); it != m_model->meshes.cend(); ++it)
		{
			if (it == m_model->meshes.cbegin())
			{
				sphere = (*it)->boundingSphere;
				box = (*it)->boundingBox;
			}
			else
			{
				BoundingSphere::CreateMerged(sphere, sphere, (*it)->boundingSphere);
				BoundingBox::CreateMerged(box, box, (*it)->boundingBox);
			}
		}

		if (sphere.Radius < 1.f)
		{
			sphere.Center = box.Center;
			sphere.Radius = std::max(box.Extents.x, std::max(box.Extents.y, box.Extents.z));
		}

		if (sphere.Radius < 1.f)
		{
			sphere.Center = XMFLOAT3(0.f, 0.f, 0.f);
			sphere.Radius = 10.f;
		}

		m_gridScale = sphere.Radius;

		m_distance = sphere.Radius * 2;

		m_cameraFocus = sphere.Center;
	}

	Vector3 dir = Vector3::Transform((m_lhcoords) ? Vector3::Forward : Vector3::Backward, m_cameraRot);
	Vector3 up = Vector3::Transform(Vector3::Up, m_cameraRot);

	m_lastCameraPos = m_cameraFocus + (m_distance * m_zoom) * dir;
}

void Game::CycleBackgroundColor()
{
	if (m_clearColor == Vector4(c_CornflowerBlue.v))
	{
		m_clearColor = Colors::Black.v;
		m_uiColor = Colors::Yellow;
	}
	else if (m_clearColor == Vector4(Colors::Black.v))
	{
		m_clearColor = Colors::White.v;
		m_uiColor = Colors::Black.v;
	}
	else
	{
		m_clearColor = c_CornflowerBlue.v;
		m_uiColor = Colors::White.v;
	}
}

void Game::CycleToneMapOperator()
{
	if (m_deviceResources->GetColorSpace() != DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709)
		return;

	m_toneMapMode += 1;

	if (m_toneMapMode >= ToneMapPostProcess::Operator_Max)
	{
		m_toneMapMode = ToneMapPostProcess::Saturate;
	}
}

void Game::CreateProjection()
{
	auto size = m_deviceResources->GetOutputSize();

	if (m_lhcoords)
	{
		m_proj = XMMatrixPerspectiveFovLH(m_fov, float(size.right) / float(size.bottom), 0.1f, m_farPlane);
	}
	else
	{
		m_proj = Matrix::CreatePerspectiveFieldOfView(m_fov, float(size.right) / float(size.bottom), 0.1f, m_farPlane);
	}

	m_lineEffect->SetProjection(m_proj);
}

void Game::RotateView(Quaternion& q)
{
	UNREFERENCED_PARAMETER(q);
	// TODO -
}