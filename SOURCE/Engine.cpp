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

#include "Constants.h"
#include "ItemData.h"
#include "WaitForGPU.h"

#include <experimental/filesystem>

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

namespace
{
	const XMVECTORF32 c_Gray = { 0.215861f, 0.215861f, 0.215861f, 1.f };
	const XMVECTORF32 c_CornflowerBlue = { 0.127438f, 0.300544f, 0.846873f, 1.f };
}

/* Create! */
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
	//Read in track config
	std::string filepath = m_filepath.generateFilepath("GAME_CORE", m_filepath.CONFIG);
	std::ifstream i(filepath);
	game_config << i;

	//We don't support more than 4 players
	if (game_config["player_count"] > 4) {
		throw std::exception("CANNOT HAVE MORE THAN FOUR PLAYERS");
	}

	//Initialise srand
	srand(std::time(NULL));

	//Initialise our device resources
	m_render_data.m_deviceResources = std::make_unique<DX::DeviceResources>(DXGI_FORMAT_R10G10B10A2_UNORM, DXGI_FORMAT_D32_FLOAT, 2,
		D3D_FEATURE_LEVEL_11_0, DX::DeviceResources::c_EnableHDR);
	m_render_data.m_deviceResources->RegisterDeviceNotify(this);

	//Initialise our HDR scene
	m_render_data.m_hdrScene = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R16G16B16A16_FLOAT);
}

/* Destroy! */
ThICC_Engine::~ThICC_Engine()
{
	//Destroy our audio engine
	if (m_audEngine)
	{
		m_audEngine->Suspend();
	}

	//Wait for our GPU process to finish before exiting
	if (m_render_data.m_deviceResources)
	{
		m_render_data.m_deviceResources->WaitForGpu();
	}
}

/* Initialize the Direct3D resources required to run. */
void ThICC_Engine::Initialize(HWND window, int width, int height)
{
	//Initialise inputs
	m_input_data.m_gamepad = std::make_unique<GamePad>();
	m_input_data.m_keyboard = std::make_unique<Keyboard>();
	m_input_data.m_mouse = std::make_unique<Mouse>();

	//Pass out stuff to our service locator
	Locator::setupID(&m_input_data);
	Locator::setupRD(&m_render_data);
	Locator::setupGSD(&m_gamestate_data);

	//Setup itembox respawn time
	ItemBoxConfig::respawn_time = game_config["itembox_respawn_time"];

	//Set window info & configure our mouse's window settings
	m_render_data.m_deviceResources->SetWindow(window, width, height);
	m_input_data.m_mouse->SetWindow(window);

	//Create resources
	m_render_data.m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	//Create window-size specific resources
	m_render_data.m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	//Configure localisation
	m_localiser.configure(game_config["language"]);

	//Setup keybinds
	m_keybinds.setup(&m_input_data);

	//Setup item data
	//m_probabilities = new ItemData();
	//Locator::setupItemData(m_probabilities);
}

/* The game update, split it out to update and render :) */
void ThICC_Engine::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

/* Update the scene */
void ThICC_Engine::Update(DX::StepTimer const& timer)
{
	//Wait if requested
	if (WaitForGPU::should_wait) {
		/* This can be massively refactored now, since WaitForGpu is available in our service locator, which is a more efficient implementation. */
		std::cout << "CALL TO WaitForGPU - THIS WILL SOON BE DEPRECIATED!";
		//--
		m_render_data.m_deviceResources->WaitForGpu();
		WaitForGPU::should_wait = false;
	}

	if (m_reloadModel)
		LoadModel("MARIOKARTSTADIUM");

	//Get keyboard and mouse state
	m_input_data.m_prevKeyboardState = m_input_data.m_keyboardState; // keep previous state for just pressed logic
	m_input_data.m_keyboardState = m_input_data.m_keyboard->GetState();
	m_input_data.m_mouseState = m_input_data.m_mouse->GetState();

	//Update trackers - this would be a nice system to move across to with the new keybind manager
	m_input_data.m_keyboardTracker.Update(m_input_data.m_keyboardState);
	m_input_data.m_mouseButtonTracker.Update(m_input_data.m_mouseState);

	//Get controller state for each player
	for (int i = 0; i < game_config["player_count"]; ++i)
	{
		m_input_data.m_gamePadState[i] = m_input_data.m_gamepad->GetState(i); //set game controllers state[s]
	}

	//Delta time
	m_gamestate_data.m_dt = float(timer.GetElapsedSeconds());

	//Pass off to our game now we've done our engine-y stuff
	m_game_inst.Update(timer);

	/* DEBUG: LOAD MODEL ON PRESS OF "O" */
	if (m_input_data.m_keyboardTracker.pressed.O)
	{
		m_reloadModel = true;
	}
}

/* Render the scene */
void ThICC_Engine::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	// Prepare the command list to render a new frame.
	m_render_data.m_deviceResources->Prepare();

	auto commandList = m_render_data.m_deviceResources->GetCommandList();
	m_render_data.m_hdrScene->BeginScene(commandList);

	Clear();

	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), m_render_data.m_states->Heap() };
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
					pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, m_render_data.m_states->AnisotropicClamp());
				}
			}
		}

		Model::UpdateEffectMatrices(m_gameMapEffects, m_world, m_view, m_proj);
		m_gameMap->Draw(commandList, m_gameMapEffects.cbegin());
	}

	m_game_inst.Render();

	m_render_data.m_hdrScene->EndScene(commandList);

	auto rtvDescriptor = m_render_data.m_deviceResources->GetRenderTargetView();
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);

	m_toneMapACESFilmic->Process(commandList);

	// Show the new frame.
	m_render_data.m_deviceResources->Present();
	m_graphicsMemory->Commit(m_render_data.m_deviceResources->GetCommandQueue());
}

// Helper method to clear the back buffers.
void ThICC_Engine::Clear()
{
	auto commandList = m_render_data.m_deviceResources->GetCommandList();

	// Clear the views.
	auto rtvDescriptor = m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene);
	auto dsvDescriptor = m_render_data.m_deviceResources->GetDepthStencilView();

	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	commandList->ClearRenderTargetView(rtvDescriptor, c_CornflowerBlue, 0, nullptr);
	commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto viewport = m_render_data.m_deviceResources->GetScreenViewport();
	auto scissorRect = m_render_data.m_deviceResources->GetScissorRect();
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);
}
#pragma endregion

/* Message handlers */
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
	auto r = m_render_data.m_deviceResources->GetOutputSize();
	m_render_data.m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}
void ThICC_Engine::OnWindowSizeChanged(int width, int height)
{
	if (!m_render_data.m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();
}

/* Window properties */
void ThICC_Engine::GetDefaultSize(int& width, int& height) const
{
	width = game_config["window_width"];
	height = game_config["window_height"];
}

/* These are the resources that depend on the device. */
void ThICC_Engine::CreateDeviceDependentResources()
{
	auto device = m_render_data.m_deviceResources->GetD3DDevice();

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

	m_render_data.m_hdrScene->SetDevice(device, m_resourceDescriptors->GetCpuHandle(Descriptors::SceneTex), m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene));

	m_render_data.m_states = std::make_unique<CommonStates>(device);

	m_lineBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(device);

	RenderTargetState rtState(m_render_data.m_deviceResources->GetBackBufferFormat(), DXGI_FORMAT_UNKNOWN);

	m_toneMapACESFilmic = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::ACESFilmic, ToneMapPostProcess::SRGB);
	m_toneMapACESFilmic->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex));

	RenderTargetState hdrState(m_render_data.m_hdrScene->GetFormat(), m_render_data.m_deviceResources->GetDepthBufferFormat());

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

	auto uploadResourcesFinished = resourceUpload.End(m_render_data.m_deviceResources->GetCommandQueue());

	m_render_data.m_deviceResources->WaitForGpu();

	uploadResourcesFinished.wait();
}

/* Allocate all memory resources that change on a window SizeChanged event. */
void ThICC_Engine::CreateWindowSizeDependentResources()
{
	auto device = m_render_data.m_deviceResources->GetD3DDevice();

	auto size = m_render_data.m_deviceResources->GetOutputSize();

	ResourceUploadBatch resourceUpload(device);

	resourceUpload.Begin();

	wchar_t consolasFont[_MAX_PATH] = {};
	wchar_t comicFont[_MAX_PATH] = {};

	DX::FindMediaFile(consolasFont, _MAX_PATH, (size.bottom > 1200) ? L"DATA/IMPORTED/consolas4k.spritefont" : L"DATA/IMPORTED/consolas.spritefont");
	DX::FindMediaFile(comicFont, _MAX_PATH, (size.bottom > 1200) ? L"DATA/IMPORTED/comic4k.spritefont" : L"DATA/IMPORTED/comic.spritefont");


	m_render_data.m_deviceResources->WaitForGpu();

	auto uploadResourcesFinished = resourceUpload.End(m_render_data.m_deviceResources->GetCommandQueue());

	uploadResourcesFinished.wait();

	m_render_data.m_hdrScene->SetWindow(size);

	m_ballCamera.SetWindow(size.right, size.bottom);
	m_ballModel.SetWindow(size.right, size.bottom);

	CreateProjection();
}

/* When we lose our device, reset everything. */
void ThICC_Engine::OnDeviceLost()
{
	m_render_data.m_gameMapPBRFactory.reset();
	m_gameMapResources.reset();
	m_gameMap.reset();
	m_gameMapEffects.clear();

	m_lineEffect.reset();
	m_lineBatch.reset();

	m_render_data.m_states.reset();

	m_toneMapACESFilmic.reset();

	m_resourceDescriptors.reset();
	m_renderDescriptors.reset();

	m_render_data.m_hdrScene->ReleaseDevice();

	m_graphicsMemory.reset();
}

/* If the device comes back, create everything again! */
void ThICC_Engine::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}

/* Load a model */
void ThICC_Engine::LoadModel(std::string filename)
{
	//Clear out existing data
	m_gameMapEffects.clear();
	m_gameMapResources.reset();
	m_gameMap.reset();
	m_render_data.m_gameMapPBRFactory.reset();

	m_render_data.m_deviceResources->WaitForGpu();

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
		auto device = m_render_data.m_deviceResources->GetD3DDevice();

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
			m_render_data.m_gameMapPBRFactory = std::make_unique<PBREffectFactory>(m_gameMapResources->Heap(), m_render_data.m_states->Heap());
			fxFactory = m_render_data.m_gameMapPBRFactory.get();

			RenderTargetState hdrState(m_render_data.m_hdrScene->GetFormat(), m_render_data.m_deviceResources->GetDepthBufferFormat());

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

		auto uploadResourcesFinished = resourceUpload.End(m_render_data.m_deviceResources->GetCommandQueue());

		m_render_data.m_deviceResources->WaitForGpu();

		uploadResourcesFinished.wait();
	}
}

/* Create a projection for our view */
void ThICC_Engine::CreateProjection()
{
	auto size = m_render_data.m_deviceResources->GetOutputSize();

	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fov, float(size.right) / float(size.bottom), 0.1f, m_farPlane);
	m_lineEffect->SetProjection(m_proj);
}