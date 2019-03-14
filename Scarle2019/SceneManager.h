#pragma once

#include "Scene.h"
#include "StepTimer.h"
#include "Constants.h"
#include "MenuScene.h"
#include "GameScene.h"
#include <vector>

class SceneManager
{
public:

	SceneManager();
	~SceneManager();

	void configure(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD);

	void addScene(Scene* _scene, Scenes _scene_name);
	void setCurrentScene(Scenes _scene_name);
	Scenes getCurrentScene();

	void Update();
	void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList);

private:
	RenderData*  m_RD;
	GameStateData* m_GSD;
	InputData* m_ID;
	WindowData* m_WD;

	Scene** m_scenes = nullptr;
	static Scenes m_curr_scene;
};

