#include "pch.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"

SceneManager::SceneManager()
{

}

int SceneManager::Update(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	if (currScene != nullptr)
	{
		switch (currScene->Update(_GSD, _ID))
		{
		case Scenes::NONE:
			return 0;
		case Scenes::MENUSCENE:
			ChangeScene(new MenuScene, _GSD, _RD, _ID, _WD);
			break;
		case Scenes::GAMESCENE:
			ChangeScene(new GameScene, _GSD, _RD, _ID, _WD);
			break;
		default:
			return -1;
			break;
		}
	}
	return 1;
}

void SceneManager::Render(RenderData* _RD, WindowData* _WD)
{
	currScene->Render(_RD, _WD);
}

void SceneManager::ChangeScene(Scene* _newScene, GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	delete currScene;
	currScene = _newScene;
	currScene->Load(_GSD, _RD, _ID, _WD);
}
