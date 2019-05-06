#pragma once

#include "pch.h"
#include "Camera.h"
#include "StepTimer.h"
#include "Scene.h"
#include "PhysModel.h"
#include "Scene.h"
#include "LocalisationManager.h"
#include <vector>

class LoadingScene : public Scene
{
public:
	LoadingScene();
	~LoadingScene() override {};

	//Core update/render/load functions
	void Update(DX::StepTimer const& timer) override;
	void Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override {};
	void Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override;
	bool Load() override { return true; };

	//Load/unload the expensive objects in the scene (only keep expensive stuff in memory if we're active)
	void ExpensiveLoad() override {};
	void ExpensiveUnload() override {};

private:

	void create2DObjects() override {};
	void create3DObjects() override {};
	void pushBackObjects() override {};
	float time = 0.0f;
	//Referenced stuff
	SceneManager* m_scene_manager;
	ImageGO2D* m_background = nullptr;
};