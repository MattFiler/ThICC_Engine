//--------------------------------------------------------------------------------------
// File: Game.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Engine.h"

#include "FindMedia.h"
#include "ReadData.h"
#include "SDKMesh.h"

#include "AssetFilepaths.h"
#include <experimental/filesystem>

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

namespace
{
	const XMVECTORF32 c_Gray = { 0.215861f, 0.215861f, 0.215861f, 1.f };
	const XMVECTORF32 c_CornflowerBlue = { 0.127438f, 0.300544f, 0.846873f, 1.f };
}

ThICC_Engine::ThICC_Engine() noexcept(false) :
	m_fov(XM_PI / 4.f),
	m_zoom(1.f),
	m_distance(10.f),
	m_farPlane(10000.f),
	m_sensitivity(1.f),
	m_deleteModel(false),
	m_reloadModel(false)
{
	Locator::setGraphicsResources(&m_graphics_resources);
	//Locator::setModelResources(&m_model_resources);

	m_graphics_resources.m_deviceResources = std::make_unique<DX::DeviceResources>(DXGI_FORMAT_R10G10B10A2_UNORM, DXGI_FORMAT_D32_FLOAT, 2,
		D3D_FEATURE_LEVEL_11_0, DX::DeviceResources::c_EnableHDR);
	m_graphics_resources.m_deviceResources->RegisterDeviceNotify(this);

	m_graphics_resources.m_hdrScene = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R16G16B16A16_FLOAT);
}

ThICC_Engine::~ThICC_Engine()
{
	if (m_graphics_resources.m_deviceResources)
	{
		m_graphics_resources.m_deviceResources->WaitForGpu();
	}
}

// Initialize the Direct3D resources required to run.
void ThICC_Engine::Initialize(HWND window, int width, int height)
{
	m_gamepad = std::make_unique<GamePad>();
	m_keyboard = std::make_unique<Keyboard>();
	m_mouse = std::make_unique<Mouse>();

	m_graphics_resources.m_deviceResources->SetWindow(window, width, height);
	m_mouse->SetWindow(window);

	m_graphics_resources.m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_graphics_resources.m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();
}

#pragma region Frame Update
// Executes the basic game loop.
void ThICC_Engine::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
		m_game_instance.Update(m_timer);
	});

	Render();
	m_game_instance.Render();
}

// Updates the world.
void ThICC_Engine::Update(DX::StepTimer const& timer)
{
	if (m_deleteModel) {
		m_model_test.reset();

		m_deleteModel = false;
	}
	if (m_reloadModel)
	{
		m_model_test.load("MARIOKARTSTADIUM");
		m_reloadModel = false;
	}

	float elapsedTime = float(timer.GetElapsedSeconds());

	auto gpad = m_gamepad->GetState(0);
	if (gpad.IsConnected())
	{
		m_gamepadButtonTracker.Update(gpad);

		// Translate camera
		Vector3 move;

		m_zoom -= gpad.thumbSticks.leftY * elapsedTime * m_sensitivity;
		m_zoom = std::max(m_zoom, 0.01f);

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
		m_graphics_resources.m_view.Invert(im);
		move = Vector3::TransformNormal(move, im);

		// Rotate camera
		Vector3 orbit(gpad.thumbSticks.rightX, gpad.thumbSticks.rightY, gpad.thumbSticks.leftX);
		orbit *= elapsedTime * m_sensitivity;

		m_cameraRot *= Quaternion::CreateFromAxisAngle(im.Right(), orbit.y);
		m_cameraRot *= Quaternion::CreateFromAxisAngle(im.Up(), -orbit.x);
		m_cameraRot *= Quaternion::CreateFromAxisAngle(im.Forward(), orbit.z);
		m_cameraRot.Normalize();

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
			//PostMessage(m_deviceResources->GetWindowHandle(), WM_USER, 0, 0);
			m_reloadModel = true;
		}

		if (m_gamepadButtonTracker.leftStick == GamePad::ButtonStateTracker::PRESSED)
		{
			CameraHome();
			m_modelRot = Quaternion::Identity;
		}
	}
	else
	{
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
			move.z += scale;

		if (kb.PageDown || kb.S)
			move.z -= scale;

		Matrix im;
		m_graphics_resources.m_view.Invert(im);
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

		m_keyboardTracker.Update(kb);

		if (m_keyboardTracker.pressed.O)
		{
			//PostMessage(m_deviceResources->GetWindowHandle(), WM_USER, 0, 0);
			m_reloadModel = true;
		}
		if (m_keyboardTracker.pressed.D)
		{
			m_deleteModel = true;
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
				delta = Vector3(0.f, 0.f, -float(mouse.y)) * m_distance * elapsedTime;
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
	Vector3 dir = Vector3::Transform(Vector3::Backward, m_cameraRot);
	Vector3 up = Vector3::Transform(Vector3::Up, m_cameraRot);

	m_lastCameraPos = m_cameraFocus + (m_distance * m_zoom) * dir;

	Locator::getGraphicsResources()->m_view = Matrix::CreateLookAt(m_lastCameraPos, m_cameraFocus, up);

	Locator::getGraphicsResources()->m_world = Matrix::CreateFromQuaternion(m_modelRot);
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void ThICC_Engine::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	// Prepare the command list to render a new frame.
	m_graphics_resources.m_deviceResources->Prepare();

	auto commandList = m_graphics_resources.m_deviceResources->GetCommandList();
	m_graphics_resources.m_hdrScene->BeginScene(commandList);

	Clear();

	ID3D12DescriptorHeap* heaps[] = { m_graphics_resources.m_resourceDescriptors->Heap(), m_graphics_resources.m_states->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	m_model_test.render();

	m_graphics_resources.m_hdrScene->EndScene(commandList);

	auto rtvDescriptor = m_graphics_resources.m_deviceResources->GetRenderTargetView();
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);

	m_graphics_resources.m_toneMapACESFilmic->Process(commandList);

	// Show the new frame.
	m_graphics_resources.m_deviceResources->Present();
	m_graphics_resources.m_graphicsMemory->Commit(m_graphics_resources.m_deviceResources->GetCommandQueue());
}

// Helper method to clear the back buffers.
void ThICC_Engine::Clear()
{
	auto commandList = m_graphics_resources.m_deviceResources->GetCommandList();

	// Clear the views.
	auto rtvDescriptor = m_graphics_resources.m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene);
	auto dsvDescriptor = m_graphics_resources.m_deviceResources->GetDepthStencilView();

	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	commandList->ClearRenderTargetView(rtvDescriptor, c_CornflowerBlue, 0, nullptr);
	commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto viewport = m_graphics_resources.m_deviceResources->GetScreenViewport();
	auto scissorRect = m_graphics_resources.m_deviceResources->GetScissorRect();
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void ThICC_Engine::OnActivated()
{
	m_keyboardTracker.Reset();
	m_mouseButtonTracker.Reset();
	m_gamepadButtonTracker.Reset();
}

void ThICC_Engine::OnDeactivated()
{
}

void ThICC_Engine::OnSuspending()
{
}

void ThICC_Engine::OnResuming()
{
	m_timer.ResetElapsedTime();
	m_keyboardTracker.Reset();
	m_mouseButtonTracker.Reset();
	m_gamepadButtonTracker.Reset();
}

void ThICC_Engine::OnWindowMoved()
{
	auto r = m_graphics_resources.m_deviceResources->GetOutputSize();
	m_graphics_resources.m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void ThICC_Engine::OnWindowSizeChanged(int width, int height)
{
	if (!m_graphics_resources.m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();
}

// Properties
void ThICC_Engine::GetDefaultSize(int& width, int& height) const
{
	width = 1280;
	height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void ThICC_Engine::CreateDeviceDependentResources()
{
	auto device = m_graphics_resources.m_deviceResources->GetD3DDevice();

	m_graphics_resources.m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

	m_graphics_resources.m_resourceDescriptors = std::make_unique<DescriptorPile>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		Locator::getGraphicsResources()->Descriptors::Count,
		Locator::getGraphicsResources()->Descriptors::Reserve);

	m_graphics_resources.m_renderDescriptors = std::make_unique<DescriptorHeap>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		RTVDescriptors::RTVCount);

	m_graphics_resources.m_hdrScene->SetDevice(
		device, m_graphics_resources.m_resourceDescriptors->GetCpuHandle(Locator::getGraphicsResources()->Descriptors::SceneTex), 
		m_graphics_resources.m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene)
	);

	m_graphics_resources.m_states = std::make_unique<CommonStates>(device);

	m_graphics_resources.m_lineBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(device);

	RenderTargetState rtState(m_graphics_resources.m_deviceResources->GetBackBufferFormat(), DXGI_FORMAT_UNKNOWN);

	m_graphics_resources.m_toneMapACESFilmic = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::ACESFilmic, ToneMapPostProcess::SRGB);
	m_graphics_resources.m_toneMapACESFilmic->SetHDRSourceTexture(m_graphics_resources.m_resourceDescriptors->GetGpuHandle(Locator::getGraphicsResources()->Descriptors::SceneTex));

	RenderTargetState hdrState(m_graphics_resources.m_hdrScene->GetFormat(), m_graphics_resources.m_deviceResources->GetDepthBufferFormat());

	{
		EffectPipelineStateDescription pd(
			&VertexPositionColor::InputLayout,
			CommonStates::Opaque,
			CommonStates::DepthRead,
			CommonStates::CullNone,
			hdrState, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

		m_graphics_resources.m_lineEffect = std::make_unique<BasicEffect>(device, EffectFlags::VertexColor, pd);
	}

	ResourceUploadBatch resourceUpload(device);

	resourceUpload.Begin();

	{
		SpriteBatchPipelineStateDescription pd(hdrState);
	}

	for (size_t j = 0; j < m_graphics_resources.hdr_count; ++j)
	{
		wchar_t radiance[_MAX_PATH] = {};
		wchar_t irradiance[_MAX_PATH] = {};

		DX::FindMediaFile(radiance, _MAX_PATH, m_graphics_resources.s_radianceIBL[j]);
		DX::FindMediaFile(irradiance, _MAX_PATH, m_graphics_resources.s_irradianceIBL[j]);

		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, radiance, m_graphics_resources.m_radianceIBL[j].ReleaseAndGetAddressOf())
		);

		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, irradiance, m_graphics_resources.m_irradianceIBL[j].ReleaseAndGetAddressOf())
		);

		CreateShaderResourceView(device, m_graphics_resources.m_radianceIBL[j].Get(), m_graphics_resources.m_resourceDescriptors->GetCpuHandle(m_graphics_resources.Descriptors::RadianceIBL1 + j), true);
		CreateShaderResourceView(device, m_graphics_resources.m_irradianceIBL[j].Get(), m_graphics_resources.m_resourceDescriptors->GetCpuHandle(m_graphics_resources.Descriptors::IrradianceIBL1 + j), true);
	}

	auto uploadResourcesFinished = resourceUpload.End(m_graphics_resources.m_deviceResources->GetCommandQueue());

	m_graphics_resources.m_deviceResources->WaitForGpu();

	uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void ThICC_Engine::CreateWindowSizeDependentResources()
{
	auto device = m_graphics_resources.m_deviceResources->GetD3DDevice();

	auto size = m_graphics_resources.m_deviceResources->GetOutputSize();

	ResourceUploadBatch resourceUpload(device);

	resourceUpload.Begin();

	wchar_t consolasFont[_MAX_PATH] = {};
	wchar_t comicFont[_MAX_PATH] = {};

	DX::FindMediaFile(consolasFont, _MAX_PATH, (size.bottom > 1200) ? L"DATA/IMPORTED/consolas4k.spritefont" : L"DATA/IMPORTED/consolas.spritefont");
	DX::FindMediaFile(comicFont, _MAX_PATH, (size.bottom > 1200) ? L"DATA/IMPORTED/comic4k.spritefont" : L"DATA/IMPORTED/comic.spritefont");


	m_graphics_resources.m_deviceResources->WaitForGpu();

	auto uploadResourcesFinished = resourceUpload.End(m_graphics_resources.m_deviceResources->GetCommandQueue());

	uploadResourcesFinished.wait();

	m_graphics_resources.m_hdrScene->SetWindow(size);

	m_ballCamera.SetWindow(size.right, size.bottom);
	m_ballModel.SetWindow(size.right, size.bottom);

	CreateProjection();
}

void ThICC_Engine::OnDeviceLost()
{
	m_model_test.reset();

	m_graphics_resources.m_lineEffect.reset();
	m_graphics_resources.m_lineBatch.reset();

	m_graphics_resources.m_states.reset();

	m_graphics_resources.m_toneMapACESFilmic.reset();

	m_graphics_resources.m_resourceDescriptors.reset();
	m_graphics_resources.m_renderDescriptors.reset();

	m_graphics_resources.m_hdrScene->ReleaseDevice();

	m_graphics_resources.m_graphicsMemory.reset();
}

void ThICC_Engine::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion

void ThICC_Engine::CameraHome()
{
	m_mouse->ResetScrollWheelValue();
	m_zoom = 1.f;
	m_fov = XM_PI / 4.f;
	m_cameraRot = Quaternion::Identity;
	m_ballCamera.Reset();

	m_cameraFocus = Vector3::Zero;
	m_distance = 10.f;

	Vector3 dir = Vector3::Transform(Vector3::Backward, m_cameraRot);
	Vector3 up = Vector3::Transform(Vector3::Up, m_cameraRot);

	m_lastCameraPos = m_cameraFocus + (m_distance * m_zoom) * dir;
}

void ThICC_Engine::CreateProjection()
{
	auto size = m_graphics_resources.m_deviceResources->GetOutputSize();

	m_graphics_resources.m_proj = Matrix::CreatePerspectiveFieldOfView(m_fov, float(size.right) / float(size.bottom), 0.1f, m_farPlane);
	m_graphics_resources.m_lineEffect->SetProjection(m_graphics_resources.m_proj);
}