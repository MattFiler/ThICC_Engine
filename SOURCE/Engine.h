//--------------------------------------------------------------------------------------
// File: Game.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//--------------------------------------------------------------------------------------

#pragma once

#include "pch.h"
#include "StepTimer.h"
#include "ArcBall.h"
#include "RenderTexture.h"

#include "DeviceResourcesPC.h"

#include "ServiceLocator.h"
#include "Game.h"

#include "ThICC_Model.h"

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class ThICC_Engine : public DX::IDeviceNotify
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
	bool RequestHDRMode() const { return m_graphics_resources.m_deviceResources ? (m_graphics_resources.m_deviceResources->GetDeviceOptions() & DX::DeviceResources::c_EnableHDR) != 0 : false; }

private:
	ThICC_Game m_game_instance;

	ThICC_GraphicsResources m_graphics_resources;
	//ThICC_ModelResources m_model_resources;

	ThICC_Model m_model_test;

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	void CameraHome();

	void CreateProjection();

	// Device resources.

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

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
	bool m_deleteModel;

	ArcBall                                         m_ballCamera;
	ArcBall                                         m_ballModel;
};