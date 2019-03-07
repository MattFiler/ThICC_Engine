#include "pch.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"

SceneManager::SceneManager()
{

}

int SceneManager::Update(GameStateData* _GSD, InputData* _ID)
{
	if (currScene != nullptr)
	{
		switch (currScene->Update(_GSD, _ID))
		{
		case Scenes::NONE:
			return 0;
		case Scenes::MENUSCENE:
			ChangeScene(new MenuScene);
			break;
		case Scenes::GAMESCENE:
			ChangeScene(new GameScene);
			break;
		default:
			return -1;
			break;
		}
	}
	return 1;
}

void SceneManager::Render(RenderData* _RD)
{
	currScene->Render(_RD);
}

void SceneManager::ChangeScene(Scene* _newScene)
{
	delete currScene;
	currScene = _newScene;
}
