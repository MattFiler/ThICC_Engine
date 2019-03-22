#pragma once

#include "Scene.h"
#include "StepTimer.h"
#include "Constants.h"
#include "MenuScene.h"
#include "GameScene.h"
#include <vector>
#include <memory>

class SceneManager
{
public:

	SceneManager();
	~SceneManager();

	void addScene(Scene* _scene, Scenes _scene_name);
	void setCurrentScene(Scenes _scene_name);
	Scenes getCurrentScene();

	void Update();
	void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList);

private:
	RenderData*  m_RD;
	GameStateData* m_GSD;
	InputData* m_ID;
	WindowData* m_WD;

	Scene* m_scene_mem_1 = nullptr;
	Scene* m_scene_mem_2 = nullptr;
	static Scenes m_curr_scene;
	static Scenes m_new_scene;
	bool did_just_update_1 = false;
	bool did_just_update_2 = false;
	bool is_safe_1 = false;
	bool is_safe_2 = false;
};

