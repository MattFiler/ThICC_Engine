#pragma once

#include "StepTimer.h"
#include "RenderTexture.h"

#include "DeviceResourcesPC.h"

#include "GameFilepaths.h"
#include "RenderData.h"
#include "DeviceData.h"
#include "CameraData.h"
#include "GameStateData.h"
#include "LocalisationManager.h"
#include "RaceManager.h"
#include "DebugConsole.h"

#include "Game.h"

#include "InputData.h"
#include "AudioManager.h"

#include <json.hpp>
using json = nlohmann::json;


// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class ThICC_Engine
	: public DX::IDeviceNotify
{
public:

	ThICC_Engine() noexcept(false);
	~ThICC_Engine();

	// Initialization and management
	void Initialize(HWND window, int width, int height);

	float SetRectHeight(int rect_num);

	float SetRectX(int rect_num);

	float SetRectY(int rect_num);

	// Basic game loop
	void Tick();

	// IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowMoved();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;
	bool RequestHDRMode() const { return m_device_data.m_deviceResources ? (m_device_data.m_deviceResources->GetDeviceOptions() & DX::DeviceResources::c_EnableHDR) != 0 : false; }

private:
	//Core game loops
	void Update(DX::StepTimer const& timer);
	void Render();

	//Create splitscreen viewports
	void SetupSplitscreenViewports();

	//Render clear
	void Clear();

	//Core resource creation
	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	//Set the font we'll use
	void SetDefaultFont(std::string _default_font);

	//Engine functions
	GameFilepaths m_filepath;
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;
	AudioManager m_AM;
	DebugConsole* m_debug_console = nullptr;
	
	//Engine data & the core game instance
	ThICC_Game m_game_inst;
	ThICC_InputData m_input_data;
	ThICC_DeviceData m_device_data;
	ThICC_GameStateData m_gamestate_data;
	ThICC_CameraData m_camera_data;

	float SetViewportWidth(int viewport_num);
	float SetViewportHeight(int viewport_num);
	float SetViewportX(int viewport_num);
	float SetViewportY(int viewport_num);

	float SetRectWidth(int rect_num);

	//Misc data banks
	json m_game_config;
	ItemData* m_probabilities = nullptr;

	//Audio engine
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
	std::unique_ptr<RaceManager> m_raceManager;

	//Game timer
	DX::StepTimer m_timer;

	std::unique_ptr<DirectX::DescriptorHeap> m_renderDescriptors; //This needs looking into - does this ever get used anymore ?!?!?

	int tonemapIndex = -1;
	bool didDeviceDisconnect = false;
	bool didShowDisconnectNotice = false;

	//Our tonemap
	std::unique_ptr<DirectX::ToneMapPostProcess> m_toneMapACESFilmic;

	//Debug text for FPS
	Text2D* debug_text = nullptr;

	//Used for internal switches between scene types
	enum RTVDescriptors
	{
		HDRScene,
		RTVCount
	};
};