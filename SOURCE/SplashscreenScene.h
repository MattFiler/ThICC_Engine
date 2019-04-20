#pragma once

#include "pch.h"
#include "Camera.h"
#include "StepTimer.h"
#include "Scene.h"
#include "PhysModel.h"
#include "ImageGO2D.h"
#include "Text2D.h"
#include <vector>

class SplashscreenScene : public Scene
{
public:
	SplashscreenScene();
	~SplashscreenScene() override;

	//Core update/render/load functions
	void Update(DX::StepTimer const& timer) override;
	void Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override {};
	void Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override;
	bool Load() override;

	//Load/unload the expensive objects in the scene (only keep expensive stuff in memory if we're active)
	void ExpensiveLoad() override {};
	void ExpensiveUnload() override {};

private:
	//Create the scene's objects
	void create2DObjects() override;
	void create3DObjects() override {};
	void pushBackObjects() override {};

	//Structures for the scene's objects
	std::vector<GameObject2D*> m_2DObjects;

	//Engine features
	KeybindManager m_keybinds;

	//References
	SceneManager* m_scene_manager;

	//Timing
	float m_timeout = 5.0f;
	float m_timer = 0.0f;

	//Logo
	ImageGO2D* m_logo = nullptr;
};

