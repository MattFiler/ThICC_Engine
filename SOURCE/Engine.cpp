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
	m_camera_data.init();

	//Pass out stuff to our service locator
	Locator::setupID(&m_input_data);
	Locator::setupDD(&m_device_data);
	Locator::setupGSD(&m_gamestate_data);
	Locator::setupAudio(&m_AM);
	Locator::setupCamData(&m_camera_data);

	// Setup Race Manager
	m_raceManager = std::make_unique<RaceManager>();
	Locator::setupRM(m_raceManager.get());

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
	Locator::getRD()->m_base_res_scale = m_game_config["ui_scale"];

	//Set our default font
	SetDefaultFont(m_game_config["font"]);

	//Setup item data
	m_probabilities = new ItemData();
	Locator::setupItemData(m_probabilities);

	//Setup debug text
	debug_text = new Text2D("", Text2D::MIDDLE);
	debug_text->SetPos(Vector2(50, 50));
	debug_text->SetColour(Colors::Red);

	//Initialise anything we need in our game
	m_game_inst.Initialize();

	//Setup viewports for splitscreen mode
	SetupSplitscreenViewports();

	//Load skybox object
	Locator::getRD()->skybox = new Skybox();

	//Create debug console
	#ifdef _DEBUG 
	m_debug_console = new DebugConsole();
	#endif
}

/* Setup our splitscreen viewport sizes */
void ThICC_Engine::SetupSplitscreenViewports() {
	int player_count = Locator::getRM()->player_amount;
	for (int i = 0; i < (player_count < 3 ? player_count : 4); i++) {
		*&Locator::getRD()->m_splitscreenViewport[i] = {
			(float)(Locator::getRD()->m_window_width * SetViewportX(i)),
			(float)(Locator::getRD()->m_window_height * SetViewportY(i)),
			(float)(Locator::getRD()->m_window_width * SetViewportWidth(i)),
			(float)(Locator::getRD()->m_window_height * SetViewportHeight(i)),
			D3D12_MIN_DEPTH, D3D12_MAX_DEPTH
		};
		*&Locator::getRD()->m_splitscreenScissorRect[i] = {
			(int)(Locator::getRD()->m_window_width * SetRectX(i)),
			(int)(Locator::getRD()->m_window_height * SetRectY(i)),
			(int)(Locator::getRD()->m_window_width * SetRectWidth(i)),
			(int)(Locator::getRD()->m_window_height * SetRectHeight(i))
		};
	}
}

float ThICC_Engine::SetViewportWidth(int viewport_num) {
	if (Locator::getRM() && Locator::getRM()->player_amount > 1)
	{
		return 0.5f;
	}

	return 1.0f;
}

float ThICC_Engine::SetViewportHeight(int viewport_num) {
	if (Locator::getRM() && Locator::getRM()->player_amount > 1)
	{
		if (Locator::getRM()->player_amount == 3 || Locator::getRM()->player_amount == 4)
			return 0.5f;
	}

	return 1.0f;
}

float ThICC_Engine::SetViewportX(int viewport_num) {
	if (Locator::getRM() && Locator::getRM()->player_amount > 1)
	{
		if (Locator::getRM()->player_amount == 2)
		{
			if (viewport_num == 1)
				return 0.5f;
		}
		else if (Locator::getRM()->player_amount == 3 || Locator::getRM()->player_amount == 4)
		{
			if (viewport_num == 1 || viewport_num == 3)
				return 0.5f;
		}
	}

	return 0.0f;
}

float ThICC_Engine::SetViewportY(int viewport_num) {
	if (Locator::getRM() && Locator::getRM()->player_amount > 1)
	{
		if (Locator::getRM()->player_amount == 3 || Locator::getRM()->player_amount == 4)
		{
			if (viewport_num == 2 || viewport_num == 3)
				return 0.5f;
		}
	}

	return 0.0f;
}

float ThICC_Engine::SetRectWidth(int rect_num) {
	if (Locator::getRM() && Locator::getRM()->player_amount > 1)
	{
		if (Locator::getRM()->player_amount == 2)
		{
			if (rect_num == 0)
				return 0.5f;
		}
		if (Locator::getRM()->player_amount == 3 || Locator::getRM()->player_amount == 4)
		{
			if (rect_num == 0 || rect_num == 2)
				return 0.5f;
		}
	}

	return 1.0f;
}

float ThICC_Engine::SetRectHeight(int rect_num) {
	if (Locator::getRM() || Locator::getRM()->player_amount > 1)
	{
		 if (Locator::getRM()->player_amount == 3 || Locator::getRM()->player_amount == 4)
		{
			if (rect_num == 0 || rect_num == 1)
				return 0.5f;
		}
	}

	return 1.0f;
}

float ThICC_Engine::SetRectX(int rect_num) {
	if (Locator::getRM() || Locator::getRM()->player_amount > 1)
	{
		if (Locator::getRM()->player_amount == 2)
		{
			if (rect_num == 1)
				return 0.5f;
		}
		else if (Locator::getRM()->player_amount == 3 || Locator::getRM()->player_amount == 4)
		{
			if (rect_num == 1 || rect_num == 3)
				return 0.5f;
		}
	}
	return 0.0f;
}

float ThICC_Engine::SetRectY(int rect_num) {
	if (Locator::getRM() || Locator::getRM()->player_amount > 1)
	{
		if (Locator::getRM()->player_amount == 3 || Locator::getRM()->player_amount == 4)
		{
			if (rect_num == 2 || rect_num == 3)
				return 0.5f;
		}
	}
	return 0.0f;
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
	if (Locator::getRM()->player_amount_changed)
	{
		SetupSplitscreenViewports();
		Locator::getRM()->player_amount_changed = false;
	}

	//Update input trackers
	m_input_data.m_keyboardTracker.Update(m_input_data.m_keyboard->GetState());
	m_input_data.m_mouseState = m_input_data.m_mouse->GetState();
	m_input_data.m_mouseButtonTracker.Update(m_input_data.m_mouseState);
	for (int i = 0; i < 4; i++) {
		m_input_data.m_gamepadButtonTracker[i].Update(m_input_data.m_gamepad->GetState(i));
	}

	//Delta time
	m_gamestate_data.m_dt = float(timer.GetElapsedSeconds());

	//Update global timer
	m_gamestate_data.m_timer = timer;

	//Framerate monitor
	debug_text->SetText(std::to_string(timer.GetFramesPerSecond()));

	//Debug console
	#ifdef _DEBUG 
	m_debug_console->Tick();
	if (m_debug_console->IsOpen()) {
		return;
	}
	#endif

	//Pass off to our game now we've done our engine-y stuff
	m_game_inst.Update(timer);
}

/* Render the scene */
void ThICC_Engine::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0 || didDeviceDisconnect)
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

	// Render the game 3D elements
	m_game_inst.Render3D();

	// Set render targets
	auto rtvDescriptor = m_device_data.m_deviceResources->GetRenderTargetView();
	commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);

	// Process our tone map
	ID3D12DescriptorHeap* heaps[] = { Locator::getRD()->m_2dResourceDescriptors->Heap(), Locator::getRD()->m_states->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	m_toneMapACESFilmic->Process(commandList);

	// Render the game 2D elements
	if (didDeviceDisconnect) return;
	m_game_inst.Render2D();

	#ifdef _DEBUG 
	try {
		Locator::getRD()->m_2dSpriteBatchFullscreen->Begin(Locator::getRD()->m_commandList.Get());
		//Debug console
		m_debug_console->Render();
		//FPS
		if (m_game_config["enable_fps"]) {
			debug_text->Render();
		}
		Locator::getRD()->m_2dSpriteBatchFullscreen->End();
	}
	catch (...) {
		DebugText::print("ThICC_Engine::Render - Failed to render, could be a sprite batch issue: most likely out of memory.");
	}
	#endif

	// Show the new frame.
	m_device_data.m_deviceResources->Present();
	Locator::getRD()->m_graphicsMemory->Commit(m_device_data.m_deviceResources->GetCommandQueue());
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
	for (int i = 0; i < 4; i++) {
		m_input_data.m_gamepadButtonTracker[i].Reset();
	}
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
	for (int i = 0; i < 4; i++) {
		m_input_data.m_gamepadButtonTracker[i].Reset();
	}
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
	#ifdef _ARCADE
	width = 320;
	height = 240;
	#else
	width = m_game_config["window_width"];
	height = m_game_config["window_height"];
	#endif
}

/* These are the resources that depend on the device. */
void ThICC_Engine::CreateDeviceDependentResources()
{
	auto device = m_device_data.m_deviceResources->GetD3DDevice();

	//Create our core render graphics resources
	Locator::getRD()->m_graphicsMemory = std::make_unique<GraphicsMemory>(device);
	m_renderDescriptors = std::make_unique<DescriptorHeap>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		RTVDescriptors::RTVCount);

	//Setup HDR scene
	tonemapIndex = Locator::getRD()->m_resourceCount;
	Locator::getRD()->m_resourceCount++;
	m_device_data.m_hdrScene->SetDevice(device, Locator::getRD()->m_2dResourceDescriptors->GetCpuHandle(tonemapIndex), m_renderDescriptors->GetCpuHandle(RTVDescriptors::HDRScene));

	//Setup states
	Locator::getRD()->m_states = std::make_unique<CommonStates>(device);

	//RT state format
	RenderTargetState rtState(m_device_data.m_deviceResources->GetBackBufferFormat(), DXGI_FORMAT_UNKNOWN);

	//Setup tone map
	m_toneMapACESFilmic = std::make_unique<ToneMapPostProcess>(device, rtState, ToneMapPostProcess::ACESFilmic, ToneMapPostProcess::SRGB);
	m_toneMapACESFilmic->SetHDRSourceTexture(Locator::getRD()->m_2dResourceDescriptors->GetGpuHandle(tonemapIndex));

	//HDR state format (required?)
	RenderTargetState hdrState(m_device_data.m_hdrScene->GetFormat(), m_device_data.m_deviceResources->GetDepthBufferFormat());
}

/* Allocate all memory resources that change on a window SizeChanged event. */
void ThICC_Engine::CreateWindowSizeDependentResources()
{
	auto size = m_device_data.m_deviceResources->GetOutputSize();
	m_device_data.m_hdrScene->SetWindow(size);

	SetupSplitscreenViewports();
}

/* When we lose our device, reset everything. */
void ThICC_Engine::OnDeviceLost()
{
	//We fail so often when trying to restore the device, alert the user.
	if (!didShowDisconnectNotice) MessageBox(NULL, (LPCWSTR)L"DX12 device removed, attempting to restore!\nApplication will likely terminate.", (LPCWSTR)L"Notice", MB_ICONERROR | MB_OK );
	didDeviceDisconnect = true;
	didShowDisconnectNotice = true;

	Locator::getRD()->m_fxFactoryPBR.reset();
	Locator::getRD()->m_states.reset();

	Locator::getRD()->m_2dFont.reset();
	Locator::getRD()->m_2dResourceDescriptors.reset();
	Locator::getRD()->m_2dSpriteBatchFullscreen.reset();
	for (int i = 0; i < 4; i++) {
		Locator::getRD()->m_2dSpriteBatchSplitscreen[i].reset();
	}

	m_toneMapACESFilmic.reset();
	m_renderDescriptors.reset();

	m_device_data.m_hdrScene->ReleaseDevice();

	Locator::getRD()->m_graphicsMemory.reset();
}

/* If the device comes back, create everything again! */
void ThICC_Engine::OnDeviceRestored()
{
	Locator::getRD()->m_d3dDevice = m_device_data.m_deviceResources->GetD3DDevice();

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	SetDefaultFont(m_game_config["font"]);
	SetupSplitscreenViewports();

	Locator::getRD()->m_2dResourceDescriptors = std::make_unique<DescriptorHeap>(Locator::getRD()->m_d3dDevice.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		1024);

	didDeviceDisconnect = false;
}

/* Set the default font */
void ThICC_Engine::SetDefaultFont(std::string _default_font)
{
	//Start the upload process
	ResourceUploadBatch resourceUpload(Locator::getRD()->m_d3dDevice.Get());
	resourceUpload.Begin();

	RenderTargetState rtState(m_device_data.m_deviceResources->GetBackBufferFormat(), DXGI_FORMAT_UNKNOWN);

	//Create sprite batches
	SpriteBatchPipelineStateDescription pd(rtState, &Locator::getRD()->m_states->NonPremultiplied);
	for (int i = 0; i < 4; i++) {
		Locator::getRD()->m_2dSpriteBatchSplitscreen[i] = std::make_unique<SpriteBatch>(Locator::getRD()->m_d3dDevice.Get(), resourceUpload, pd);
		Locator::getRD()->m_2dSpriteBatchSplitscreen[i]->SetViewport(Locator::getRD()->m_splitscreenViewport[i]);
	}
	Locator::getRD()->m_2dSpriteBatchFullscreen = std::make_unique<SpriteBatch>(Locator::getRD()->m_d3dDevice.Get(), resourceUpload, pd);
	Locator::getRD()->m_2dSpriteBatchFullscreen->SetViewport(Locator::getRD()->m_fullscreenViewport);

	//Get font name
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string font_path = m_filepath.generateFilepath(_default_font, m_filepath.FONT);
	std::wstring w_font_path = converter.from_bytes(font_path.c_str());

	//Load font resource
	Locator::getRD()->m_2dFont = std::make_unique<SpriteFont>(Locator::getRD()->m_d3dDevice.Get(), resourceUpload,
		w_font_path.c_str(),
		Locator::getRD()->m_2dResourceDescriptors->GetCpuHandle(Locator::getRD()->m_resourceCount),
		Locator::getRD()->m_2dResourceDescriptors->GetGpuHandle(Locator::getRD()->m_resourceCount));
	Locator::getRD()->m_resourceCount++;

	//End the upload process
	auto uploadResourcesFinished = resourceUpload.End(Locator::getRD()->m_commandQueue.Get());
	uploadResourcesFinished.wait();
}