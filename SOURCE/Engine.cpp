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
ThICC_Engine::ThICC_Engine() noexcept(false)
{
	//Read in track config
	std::ifstream i(m_filepath.generateFilepath("GAME_CORE", m_filepath.CONFIG));
	m_game_config << i;

	//We don't support more than 4 players
	if (m_game_config["player_count"] > 4) {
		throw std::exception("CANNOT HAVE MORE THAN FOUR PLAYERS");
	}

	//Initialise srand
	srand(std::time(NULL));

	//Initialise our device resources
	m_device_data.m_deviceResources = std::make_unique<DX::DeviceResources>(DXGI_FORMAT_R10G10B10A2_UNORM, DXGI_FORMAT_D32_FLOAT, 2,
		D3D_FEATURE_LEVEL_11_0, DX::DeviceResources::c_EnableHDR);
	m_device_data.m_deviceResources->RegisterDeviceNotify(this);

	//Initialise our HDR scene
	m_device_data.m_hdrScene = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R16G16B16A16_FLOAT);
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
	if (m_device_data.m_deviceResources)
	{
		m_device_data.m_deviceResources->WaitForGpu();
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
	Locator::setupDD(&m_device_data);
	Locator::setupGSD(&m_gamestate_data);
	Locator::setupAudio(&m_AM);

	//Setup itembox respawn time
	ItemBoxConfig::respawn_time = m_game_config["itembox_respawn_time"];

	//Set window info & configure our mouse's window settings
	m_device_data.m_deviceResources->SetWindow(window, width, height);
	m_input_data.m_mouse->SetWindow(window);

	//Create resources
	m_device_data.m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	//Create window-size specific resources
	m_device_data.m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	//Configure localisation
	m_localiser.configure(m_game_config["language"]);

	//Setup keybinds
	m_keybinds.setup(&m_input_data);

	//Save our window size config
	Locator::getRD()->m_window_height = height;
	Locator::getRD()->m_window_width = width;

	//Set our default font
	SetDefaultFont("NeueHaasGroteskDisp Pro BD");

	//Setup item data
	m_probabilities = new ItemData();
	Locator::setupItemData(m_probabilities);

	//Initialise anything we need in our game
	m_game_inst.Initialize();
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
	//Get keyboard and mouse state
	m_input_data.m_prevKeyboardState = m_input_data.m_keyboardState; // keep previous state for just pressed logic
	m_input_data.m_keyboardState = m_input_data.m_keyboard->GetState();
	m_input_data.m_mouseState = m_input_data.m_mouse->GetState();

	//Update trackers - this would be a nice system to move across to with the new keybind manager
	m_input_data.m_keyboardTracker.Update(m_input_data.m_keyboardState);
	m_input_data.m_mouseButtonTracker.Update(m_input_data.m_mouseState);

	//Get controller state for each player
	for (int i = 0; i < m_game_config["player_count"]; ++i)
	{
		m_input_data.m_gamePadState[i] = m_input_data.m_gamepad->GetState(i); //set game controllers state[s]
	}

	//Delta time
	m_gamestate_data.m_dt = float(timer.GetElapsedSeconds());

	//Pass off to our game now we've done our engine-y stuff
	m_game_inst.Update(timer);

	/* DEBUG: CHANGE ENV MAPS ON X PRESS */
	if (m_input_data.m_keyboardTracker.IsKeyPressed(Keyboard::X))
	{
		++Locator::getRD()->m_ibl;
		if (Locator::getRD()->m_ibl >= (int)NUM_OF_ENV_MAPS::ENV_MAP_COUNT)
		{
			Locator::getRD()->m_ibl = 0;
		}
		DebugText::print("Changing to environment map " + std::to_string(Locator::getRD()->m_ibl));
		DebugText::print("THIS FUNCTIONALITY NEEDS TO BE REMOVED :)");
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
	m_device_data.m_deviceResources->Prepare();
	auto commandList = m_device_data.m_deviceResources->GetCommandList();

	// Clear previous frame
	m_device_data.m_hdrScene->BeginScene(commandList); //do we need this here?
	Clear();
	m_device_data.m_hdrScene->EndScene(commandList);

	// Render the game
	m_game_inst.Render();

	// Set render targets
	auto rtvDescriptor = m_device_data.m_deviceResources->GetRenderTargetView();
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);

	// Process our tone map
	ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), Locator::getRD()->m_states->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_toneMapACESFilmic->Process(commandList);

	// Show the new frame.
	m_device_data.m_deviceResources->Present();
	m_graphicsMemory->Commit(m_device_data.m_deviceResources->GetCommandQueue());
}

// Helper method to clear the back buffers.
void ThICC_Engine::Clear()
{
	auto commandList = m_device_data.m_deviceResources->GetCommandList();

	// Clear the views.
	auto rtvDescriptor = m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene);
	auto dsvDescriptor = m_device_data.m_deviceResources->GetDepthStencilView();

	// Set clear colour and other properties
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
	commandList->ClearRenderTargetView(rtvDescriptor, c_CornflowerBlue, 0, nullptr);
	commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// Set the viewport and scissor rect.
	auto viewport = m_device_data.m_deviceResources->GetScreenViewport();
	auto scissorRect = m_device_data.m_deviceResources->GetScissorRect();
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);
}
#pragma endregion

/* Message handlers */
void ThICC_Engine::OnActivated()
{
	//Reset input trackers
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
	//Reset time and input trackers
	m_timer.ResetElapsedTime();
	m_input_data.m_keyboardTracker.Reset();
	m_input_data.m_mouseButtonTracker.Reset();
	m_input_data.m_gamepadButtonTracker.Reset();
}
void ThICC_Engine::OnWindowMoved()
{
	//Window moved
	auto r = m_device_data.m_deviceResources->GetOutputSize();
	m_device_data.m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}
void ThICC_Engine::OnWindowSizeChanged(int width, int height)
{
	//Window resized
	if (!m_device_data.m_deviceResources->WindowSizeChanged(width, height))
		return;

	CreateWindowSizeDependentResources();
}

/* Window properties */
void ThICC_Engine::GetDefaultSize(int& width, int& height) const
{
	width = m_game_config["window_width"];
	height = m_game_config["window_height"];
}

/* These are the resources that depend on the device. */
void ThICC_Engine::CreateDeviceDependentResources()
{
	auto device = m_device_data.m_deviceResources->GetD3DDevice();

	//Create our core graphics resources (the count of these can probs be reduced to save a bit of memory)
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

	//Setup HDR scene
	m_device_data.m_hdrScene->SetDevice(device, m_resourceDescriptors->GetCpuHandle(Descriptors::SceneTex), m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene));

	//Setup states
	Locator::getRD()->m_states = std::make_unique<CommonStates>(device);

	//RT state format
	RenderTargetState rtState(m_device_data.m_deviceResources->GetBackBufferFormat(), DXGI_FORMAT_UNKNOWN);

	//Setup tone map
	m_toneMapACESFilmic = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::ACESFilmic, ToneMapPostProcess::SRGB);
	m_toneMapACESFilmic->SetHDRSourceTexture(m_resourceDescriptors->GetGpuHandle(Descriptors::SceneTex));

	//HDR state format (required?)
	RenderTargetState hdrState(m_device_data.m_hdrScene->GetFormat(), m_device_data.m_deviceResources->GetDepthBufferFormat());
}

/* Allocate all memory resources that change on a window SizeChanged event. */
void ThICC_Engine::CreateWindowSizeDependentResources()
{
	auto size = m_device_data.m_deviceResources->GetOutputSize();
	m_device_data.m_hdrScene->SetWindow(size);
}

/* When we lose our device, reset everything. */
void ThICC_Engine::OnDeviceLost()
{
	Locator::getRD()->m_fxFactoryPBR.reset();

	Locator::getRD()->m_states.reset();

	m_toneMapACESFilmic.reset();

	m_resourceDescriptors.reset();
	m_renderDescriptors.reset();

	m_device_data.m_hdrScene->ReleaseDevice();

	m_graphicsMemory.reset();
}

/* If the device comes back, create everything again! */
void ThICC_Engine::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}

/* Set the default font */
void ThICC_Engine::SetDefaultFont(std::string _default_font)
{
	////GEP: Set up Sprite batch for drawing textures also loads the font for text
	Locator::getRD()->m_states = std::make_unique<CommonStates>(Locator::getRD()->m_d3dDevice.Get());
	ResourceUploadBatch resourceUpload(Locator::getRD()->m_d3dDevice.Get());

	resourceUpload.Begin();
	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	SpriteBatchPipelineStateDescription pd(rtState);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string font_path = m_filepath.generateFilepath(_default_font, m_filepath.FONT);
	std::wstring w_font_path = converter.from_bytes(font_path.c_str());
	pd.blendDesc = Locator::getRD()->m_states->NonPremultiplied;
	Locator::getRD()->m_spriteBatch = std::make_unique<SpriteBatch>(Locator::getRD()->m_d3dDevice.Get(), resourceUpload, pd);
	//This will throw an exception in <memory> if we try to load a non-existant font.
	Locator::getRD()->m_font = std::make_unique<SpriteFont>(Locator::getRD()->m_d3dDevice.Get(), resourceUpload,
		w_font_path.c_str(),
		Locator::getRD()->m_resourceDescriptors->GetCpuHandle(Locator::getRD()->m_resourceCount),
		Locator::getRD()->m_resourceDescriptors->GetGpuHandle(Locator::getRD()->m_resourceCount));
	Locator::getRD()->m_resourceCount++;
	auto uploadResourcesFinished = resourceUpload.End(Locator::getRD()->m_commandQueue.Get());
}