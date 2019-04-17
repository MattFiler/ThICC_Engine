//--------------------------------------------------------------------------------------
// File: Game.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include "ArcBall.h"
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

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	void LoadModel(std::string filename);
	void SetDefaultFont(std::string _default_font);

	void CreateProjection();

	GameFilepaths m_filepath;
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;
	AudioManager m_AM;
	
	ThICC_Game m_game_inst;
	ThICC_InputData m_input_data;
	ThICC_DeviceData m_device_data;
	ThICC_GameStateData m_gamestate_data;

	json m_game_config;
	ItemData* m_probabilities = nullptr;

	//audio system
	//This uses a simple system, but a better pipeline can be used using Wave Banks
	//See here: https://github.com/Microsoft/DirectXTK/wiki/Creating-and-playing-sounds Using wave banks Section
	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	std::unique_ptr<DirectX::GraphicsMemory>        m_graphicsMemory;
	std::unique_ptr<DirectX::DescriptorPile>        m_resourceDescriptors;
	std::unique_ptr<DirectX::DescriptorHeap>        m_renderDescriptors;

	std::unique_ptr<DirectX::BasicEffect>                                   m_lineEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_lineBatch;

	std::unique_ptr<DirectX::ToneMapPostProcess>    m_toneMapACESFilmic;

	std::unique_ptr<DirectX::EffectTextureFactory>  m_gameMapResources;
	std::unique_ptr<DirectX::Model>                 m_gameMap;
	std::vector<std::shared_ptr<DirectX::IEffect>>  m_gameMapEffects;

	enum Descriptors
	{
		ConsolasFont,
		ComicFont,
		SceneTex,
		RadianceIBL1,
		RadianceIBL2,
		RadianceIBL3,
		IrradianceIBL1,
		IrradianceIBL2,
		IrradianceIBL3,
		Reserve,
		Count = 1024
	};

	enum RTVDescriptors
	{
		HDRScene,
		RTVCount
	};

	DirectX::SimpleMath::Matrix                     m_world;
	DirectX::SimpleMath::Matrix                     m_view;
	DirectX::SimpleMath::Matrix                     m_proj;

	DirectX::SimpleMath::Vector3                    m_cameraFocus;
	DirectX::SimpleMath::Vector3                    m_lastCameraPos;
	DirectX::SimpleMath::Quaternion                 m_cameraRot;
	DirectX::SimpleMath::Quaternion                 m_viewRot;

	DirectX::SimpleMath::Quaternion                 m_modelRot;

	float                                           m_fov;
	float                                           m_zoom;
	float                                           m_distance;
	float                                           m_farPlane;
	float                                           m_sensitivity;

	bool                                            m_reloadModel;

	int                                             m_toneMapMode;

	ArcBall                                         m_ballCamera;
	ArcBall                                         m_ballModel;
};