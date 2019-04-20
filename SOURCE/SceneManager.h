#pragma once

#include "Scene.h"
#include "StepTimer.h"
#include "Constants.h"
#include <vector>

#include "MenuScene.h"
#include "GameScene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void Initialize();

	void addScene(Scene* _scene, int _scene_name);
	void setCurrentScene(int _scene_name, bool _first_load = false);

	void Update(DX::StepTimer const& timer);
	void Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList);
	void Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList);

private:
	Scene* m_curr_scene = nullptr;
	Scene* m_prev_scene = nullptr;

	std::vector<Scene*> m_scenes;
	std::vector<int> m_sceneDescriptors;

	ImageGO2D* loadscreen = nullptr;
	bool show_loadscreen = false;

	//Ugly state trackers for the loadscreen - can probably be refactored!
	int delete_counter = 0;
	bool needs_delete = false;
	int scene_to_delete = 0;
	bool scene_switch = false;
	bool first_load = false;
	int scene_to_switch = 0;
	bool switched_scene = false;
};

