#include "pch.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"

SceneManager::SceneManager()
{

}

int SceneManager::Update(double _timer)
{
	if (currScene != nullptr)
	{
		switch (currScene->Update(_timer))
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

void SceneManager::Render()
{
	currScene->Render();
}

bool SceneManager::Load()
{
	return true;
}

void SceneManager::ChangeScene(Scene* _newScene)
{
	delete currScene;
	currScene = _newScene;
}
