#pragma once

#include "StepTimer.h"
#include "Scene.h"
#include "LocalisationManager.h"
#include "Camera.h"
#include <vector>
#include <json.hpp>
using json = nlohmann::json;

class DebugScene : public Scene
{
public:
	DebugScene();
	~DebugScene() override;

	//Core update/render/load functions
	void Update(DX::StepTimer const& timer) override;
	void Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override;
	void Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override {};
	bool Load() override;

	//Load/unload the expensive objects in the scene (only keep expensive stuff in memory if we're active)
	void ExpensiveLoad() override;
	void ExpensiveUnload() override;

private:
	//Item creation
	void create2DObjects() override {};
	void create3DObjects() override;
	void pushBackObjects() override {};

	//The scene's objects
	Camera*	debug_cam = nullptr;
	std::vector<GameObject3D*> m_3DObjects;

	//Common engine components
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;
	GameFilepaths m_filepaths;

	//Our scene manager instance
	SceneManager* m_scene_manager;
};

