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


// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game
	: public DX::IDeviceNotify
{
public:

	Game() noexcept(false);
	~Game();

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
	bool RequestHDRMode() const { return m_deviceResources ? (m_deviceResources->GetDeviceOptions() & DX::DeviceResources::c_EnableHDR) != 0 : false; }

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	void LoadModel(std::string filename);

	void CameraHome();

	void CreateProjection();

	// Device resources.
	std::unique_ptr<DX::DeviceResources>            m_deviceResources;
	std::unique_ptr<DX::RenderTexture>              m_hdrScene;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	std::unique_ptr<DirectX::GraphicsMemory>        m_graphicsMemory;
	std::unique_ptr<DirectX::DescriptorPile>        m_resourceDescriptors;
	std::unique_ptr<DirectX::DescriptorHeap>        m_renderDescriptors;
	std::unique_ptr<DirectX::CommonStates>          m_states;

	std::unique_ptr<DirectX::BasicEffect>                                   m_lineEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_lineBatch;

	std::unique_ptr<DirectX::ToneMapPostProcess>    m_toneMapACESFilmic;

	std::unique_ptr<DirectX::EffectFactory>         m_fxFactory; //SDKMESH1
	std::unique_ptr<DirectX::PBREffectFactory>      m_pbrFXFactory; //SDKMESH2
	std::unique_ptr<DirectX::EffectTextureFactory>  m_modelResources;
	std::unique_ptr<DirectX::Model>                 m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>>  m_modelClockwise;

	static const size_t s_nIBL = 3;

	Microsoft::WRL::ComPtr<ID3D12Resource>          m_radianceIBL[s_nIBL];
	Microsoft::WRL::ComPtr<ID3D12Resource>          m_irradianceIBL[s_nIBL];

	enum Descriptors
	{
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

	std::unique_ptr<DirectX::GamePad>               m_gamepad;
	std::unique_ptr<DirectX::Keyboard>              m_keyboard;
	std::unique_ptr<DirectX::Mouse>                 m_mouse;

	DirectX::Keyboard::KeyboardStateTracker         m_keyboardTracker;
	DirectX::Mouse::ButtonStateTracker              m_mouseButtonTracker;
	DirectX::GamePad::ButtonStateTracker            m_gamepadButtonTracker;

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

	ArcBall                                         m_ballCamera;
	ArcBall                                         m_ballModel;
};