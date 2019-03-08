#pragma once

#include "Scene.h"
#include "StepTimer.h"
#include "Constants.h"
#include "MenuScene.h"
#include "GameScene.h"

class SceneManager
{
public:

	SceneManager(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD);

	int Update(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD);
	void Render(RenderData* _RD, WindowData* _WD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList);

	Scene* currScene = nullptr;

	Scene* curScene[2]{ nullptr };

	RenderData*  m_RD;
	GameStateData* m_GSD;
	InputData* m_ID;
	WindowData* m_WD;

private:
	void ChangeScene(Scene* _newScene);
	enum Sce {
		MENU = 0,
		GAME = 1,
	};

	Sce c_s = MENU;
};

