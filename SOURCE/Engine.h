#pragma once

#include "StepTimer.h"
#include "RenderTexture.h"

#include "DeviceResourcesPC.h"

#include "GameFilepaths.h"
#include "RenderData.h"
#include "DeviceData.h"
#include "GameStateData.h"
#include "LocalisationManager.h"

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
	
	//Engine data & the core game instance
	ThICC_Game m_game_inst;
	ThICC_InputData m_input_data;
	ThICC_DeviceData m_device_data;
	ThICC_GameStateData m_gamestate_data;

	//Misc data banks
	json m_game_config;
	ItemData* m_probabilities = nullptr;

	//Audio engine
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	//Game timer
	DX::StepTimer m_timer;

	//Core resources (these can probably be cut back a bit, since we only really use them for the tonemap now)
	std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;
	std::unique_ptr<DirectX::DescriptorHeap> m_renderDescriptors;

	int tonemapIndex = -1;

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