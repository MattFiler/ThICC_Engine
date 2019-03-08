#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	m_RD = _RD;
	m_GSD = _GSD;
	m_ID= _ID;
	m_WD = _WD;
}

int SceneManager::Update(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	//if (currScene != nullptr)
	//{
	//	switch (currScene->Update(_GSD, _ID))
	//	{
	//	case Scenes::NONE:
	//		return 0;
	//	case Scenes::MENUSCENE:
	//		new_scene_menu = new MenuScene();
	//		ChangeScene(new_scene_menu);
	//		break;
	//	case Scenes::GAMESCENE:
	//		new_scene_game = new GameScene();
	//		ChangeScene(new_scene_game);
	//		break;
	//	default:
	//		return -1;
	//		break;
	//	}
	//}
	//return 1;
	switch (curScene[int(c_s)]->Update(_GSD, _ID))
	{
	case Scenes::NONE:
		return 0;
	case Scenes::MENUSCENE:
		c_s = MENU;
		break;
	case Scenes::GAMESCENE:
		c_s = GAME;
		break;
	default:
		return -1;
		break;
	}
	return 1;
}

void SceneManager::Render(RenderData* _RD, WindowData* _WD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	//currScene->Render(_RD, _WD, m_commandList);
	curScene[int(c_s)]->Render(_RD, _WD, m_commandList);
}

void SceneManager::ChangeScene(Scene* _newScene)
{
	delete currScene;
	currScene = _newScene;
	currScene->Load(m_GSD, m_RD, m_ID, m_WD);
}
