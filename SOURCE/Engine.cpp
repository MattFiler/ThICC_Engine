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

#include "ServiceLocator.h"

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
	m_ibl(0),
	m_reloadModel(false),
	m_toneMapMode(ToneMapPostProcess::ACESFilmic)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>(DXGI_FORMAT_R10G10B10A2_UNORM, DXGI_FORMAT_D32_FLOAT, 2,
		D3D_FEATURE_LEVEL_11_0, DX::DeviceResources::c_EnableHDR);
	m_deviceResources->RegisterDeviceNotify(this);

	m_hdrScene = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R16G16B16A16_FLOAT);
}

ThICC_Engine::~ThICC_Engine()
{
	if (m_deviceResources)
	{
		m_deviceResources->WaitForGpu();
	}
}

// Initialize the Direct3D resources required to run.
void ThICC_Engine::Initialize(HWND window, int width, int height)
{
	m_input_data.m_gamepad = std::make_unique<GamePad>();
	m_input_data.m_keyboard = std::make_unique<Keyboard>();
	m_input_data.m_mouse = std::make_unique<Mouse>();

	Locator::setupID(&m_input_data);

	m_deviceResources->SetWindow(window, width, height);
	m_input_data.m_mouse->SetWindow(window);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();
}

#pragma region Frame Update
// Executes the basic game loop.
void ThICC_Engine::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void ThICC_Engine::Update(DX::StepTimer const& timer)
{
	if (m_reloadModel)
		LoadModel("MARIOKARTSTADIUM");

	DirectX::GamePad::State gpad = m_input_data.m_gamepad->GetState(0);
	if (gpad.IsConnected())
	{
		m_input_data.m_gamepadButtonTracker.Update(gpad);

		m_game_inst.InputHandler(gpad);
	}
	else
	{
		m_input_data.m_gamepadButtonTracker.Reset();

		DirectX::Keyboard::State kb = m_input_data.m_keyboard->GetState();
		DirectX::Mouse::State mouse = m_input_data.m_mouse->GetState();

		m_input_data.m_keyboardTracker.Update(kb);
		m_input_data.m_mouseButtonTracker.Update(mouse);

		m_game_inst.InputHandler(kb, mouse);

		if (m_input_data.m_keyboardTracker.pressed.O)
		{
			//PostMessage(m_deviceResources->GetWindowHandle(), WM_USER, 0, 0);
			m_reloadModel = true;
		}
	}

	m_game_inst.Update(timer);
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
	m_deviceResources->Prepare();

	auto commandList = m_deviceResources->GetCommandList();
	m_hdrScene->BeginScene(commandList);

	Clear();

	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_states->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	// Render game map if active
	if (m_gameMap)
	{
		{
			auto radianceTex = m_resourceDescriptors->GetGpuHandle(Descriptors::RadianceIBL1 + m_ibl);
			auto diffuseDesc = m_radianceIBL[0]->GetDesc();
			auto irradianceTex = m_resourceDescriptors->GetGpuHandle(Descriptors::IrradianceIBL1 + m_ibl);

			for (auto& it : m_gameMapEffects)
			{
				auto pbr = dynamic_cast<PBREffect*>(it.get());
				if (pbr)
				{
					pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, m_states->AnisotropicClamp());
				}
			}
		}

		Model::UpdateEffectMatrices(m_gameMapEffects, m_world, m_view, m_proj);
		m_gameMap->Draw(commandList, m_gameMapEffects.cbegin());
	}

	m_game_inst.Render();

	m_hdrScene->EndScene(commandList);

	auto rtvDescriptor = m_deviceResources->GetRenderTargetView();
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);

	m_toneMapACESFilmic->Process(commandList);

	// Show the new frame.
	m_deviceResources->Present();
	m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());
}

// Helper method to clear the back buffers.
void ThICC_Engine::Clear()
{
	auto commandList = m_deviceResources->GetCommandList();

	// Clear the views.
	auto rtvDescriptor = m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene);
	auto dsvDescriptor = m_deviceResources->GetDepthStencilView();

	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	commandList->ClearRenderTargetView(rtvDescriptor, c_CornflowerBlue, 0, nullptr);
	commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto viewport = m_deviceResources->GetScreenViewport();
	auto scissorRect = m_deviceResources->GetScissorRect();
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void ThICC_Engine::OnActivated()
{
	m_input_data.m_keyboardTracker.Reset();
	m_input_data.m_mouseButtonTracker.Reset();
	m_input_data.m_gamepadButtonTracker.Reset();
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
	m_input_data.m_keyboardTracker.Reset();
	m_input_data.m_mouseButtonTracker.Reset();
	m_input_data.m_gamepadButtonTracker.Reset();
}

void ThICC_Engine::OnWindowMoved()
{
	auto r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void ThICC_Engine::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
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

	m_toneMapACESFilmic = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::ACESFilmic, ToneMapPostProcess::SRGB);
	m_toneMapACESFilmic->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex));

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
	}

	static const wchar_t* s_radianceIBL[s_nIBL] =
	{
		L"DATA/IMPORTED/Atrium_diffuseIBL.dds",
		L"DATA/IMPORTED/Garage_diffuseIBL.dds",
		L"DATA/IMPORTED/SunSubMixer_diffuseIBL.dds",
	};
	static const wchar_t* s_irradianceIBL[s_nIBL] =
	{
		L"DATA/IMPORTED/Atrium_specularIBL.dds",
		L"DATA/IMPORTED/Garage_specularIBL.dds",
		L"DATA/IMPORTED/SunSubMixer_specularIBL.dds",
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
void ThICC_Engine::CreateWindowSizeDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();

	auto size = m_deviceResources->GetOutputSize();

	ResourceUploadBatch resourceUpload(device);

	resourceUpload.Begin();

	wchar_t consolasFont[_MAX_PATH] = {};
	wchar_t comicFont[_MAX_PATH] = {};

	DX::FindMediaFile(consolasFont, _MAX_PATH, (size.bottom > 1200) ? L"DATA/IMPORTED/consolas4k.spritefont" : L"DATA/IMPORTED/consolas.spritefont");
	DX::FindMediaFile(comicFont, _MAX_PATH, (size.bottom > 1200) ? L"DATA/IMPORTED/comic4k.spritefont" : L"DATA/IMPORTED/comic.spritefont");


	m_deviceResources->WaitForGpu();

	auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());

	uploadResourcesFinished.wait();

	m_hdrScene->SetWindow(size);

	m_ballCamera.SetWindow(size.right, size.bottom);
	m_ballModel.SetWindow(size.right, size.bottom);

	CreateProjection();
}

void ThICC_Engine::OnDeviceLost()
{
	m_gameMapPBRFactory.reset();
	m_gameMapResources.reset();
	m_gameMap.reset();
	m_gameMapEffects.clear();

	m_lineEffect.reset();
	m_lineBatch.reset();

	m_states.reset();

	m_toneMapACESFilmic.reset();

	m_resourceDescriptors.reset();
	m_renderDescriptors.reset();

	m_hdrScene->ReleaseDevice();

	m_graphicsMemory.reset();
}

void ThICC_Engine::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion

void ThICC_Engine::LoadModel(std::string filename)
{
	//Clear out existing data
	m_gameMapEffects.clear();
	m_gameMapResources.reset();
	m_gameMap.reset();
	m_gameMapPBRFactory.reset();

	m_deviceResources->WaitForGpu();

	m_reloadModel = false;
	m_modelRot = Quaternion::Identity;

	//Get path to SDKMESH
	std::string fullpath = m_filepath.generateFilepath(filename, m_filepath.MODEL);
	std::wstring fullpath_wstring = std::wstring(fullpath.begin(), fullpath.end());
	const wchar_t* fullpath_wchar = fullpath_wstring.c_str();

	try
	{
		//Load model binary data
		auto modelBin = DX::ReadData(fullpath_wchar);

		//Check SDKMESH version
		if (modelBin.size() >= sizeof(DXUT::SDKMESH_HEADER))
		{
			auto hdr = reinterpret_cast<const DXUT::SDKMESH_HEADER*>(modelBin.data());
			if (!(hdr->Version >= 200))
			{
				throw std::exception("SDKMESH is not V2! Model must've been imported with old toolkit.");
			}
		}

		//Load as SDKMESH
		m_gameMap = Model::CreateFromSDKMESH(modelBin.data(), modelBin.size());
	}
	catch (...)
	{
		throw std::exception("Could not load model.");
		m_gameMap.reset();
	}

	if (m_gameMap)
	{
		auto device = m_deviceResources->GetD3DDevice();

		ResourceUploadBatch resourceUpload(device);
		resourceUpload.Begin();

		m_gameMap->LoadStaticBuffers(device, resourceUpload, true);

		m_gameMapResources = std::make_unique<EffectTextureFactory>(device, resourceUpload, m_resourceDescriptors->Heap());
		
		//Get current directory (this is currently hacky and needs a fix - is VS giving us the wrong working dir?)
		std::string curr_dir = std::experimental::filesystem::current_path().string();
		if (curr_dir.substr(curr_dir.length() - 6) == "SOURCE") {
			curr_dir = curr_dir.substr(0, curr_dir.length() - 7);
		}
		std::string dirpath = curr_dir + "/" + m_filepath.getFolder(m_filepath.MODEL) + filename + "/";
		if (dirpath.length() > 7 && dirpath.substr(dirpath.length() - 6) == "DEBUG/") {
			dirpath = dirpath.substr(0, dirpath.length() - 7) + "/";
			//is_debug_mesh = true;
		}
		std::wstring dirpath_wstring = std::wstring(dirpath.begin(), dirpath.end());
		const wchar_t* dirpath_wchar = dirpath_wstring.c_str();

		m_gameMapResources->SetDirectory(dirpath_wchar);


		int txtOffset = Descriptors::Reserve;
		try
		{
			//Load materials
			(void)m_gameMap->LoadTextures(*m_gameMapResources, txtOffset);
		}
		catch (...)
		{
			throw std::exception("Could not load model's materials.");
			m_gameMap.reset();
			m_gameMapResources.reset();
		}

		if (m_gameMap)
		{
			//Create effect factory
			IEffectFactory *fxFactory = nullptr;
			m_gameMapPBRFactory = std::make_unique<PBREffectFactory>(m_gameMapResources->Heap(), m_states->Heap());
			fxFactory = m_gameMapPBRFactory.get();

			RenderTargetState hdrState(m_hdrScene->GetFormat(), m_deviceResources->GetDepthBufferFormat());

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

			m_gameMapEffects = m_gameMap->CreateEffects(*fxFactory, pd, pdAlpha, txtOffset);
		}

		auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());

		m_deviceResources->WaitForGpu();

		uploadResourcesFinished.wait();
	}

	CameraHome();
}

void ThICC_Engine::CameraHome()
{
	m_input_data.m_mouse->ResetScrollWheelValue();
	m_zoom = 1.f;
	m_fov = XM_PI / 4.f;
	m_cameraRot = Quaternion::Identity;
	m_ballCamera.Reset();

	if (!m_gameMap)
	{
		m_cameraFocus = Vector3::Zero;
		m_distance = 10.f;
	}
	else
	{
		BoundingSphere sphere;
		BoundingBox box;

		for (auto it = m_gameMap->meshes.cbegin(); it != m_gameMap->meshes.cend(); ++it)
		{
			if (it == m_gameMap->meshes.cbegin())
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

		m_distance = sphere.Radius * 2;

		m_cameraFocus = sphere.Center;
	}

	Vector3 dir = Vector3::Transform(Vector3::Backward, m_cameraRot);
	Vector3 up = Vector3::Transform(Vector3::Up, m_cameraRot);

	m_lastCameraPos = m_cameraFocus + (m_distance * m_zoom) * dir;
}

void ThICC_Engine::CreateProjection()
{
	auto size = m_deviceResources->GetOutputSize();

	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fov, float(size.right) / float(size.bottom), 0.1f, m_farPlane);
	m_lineEffect->SetProjection(m_proj);
}