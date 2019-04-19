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

	void addScene(Scene* _scene, int _scene_name);
	void setCurrentScene(int _scene_name, bool _first_load = false);

	void Update();
	void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList);

private:
	RenderData*  m_RD;
	GameStateData* m_GSD;
	InputData* m_ID;

	Scene* m_curr_scene = nullptr;
	Scene* m_prev_scene = nullptr;

	std::vector<Scene*> m_scenes;
	std::vector<int> m_sceneDescriptors;

	int delete_counter = 0;
	bool needs_delete = false;
	int scene_to_delete = 0;
};

