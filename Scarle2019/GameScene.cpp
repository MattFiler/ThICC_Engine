#include "pch.h"
#include "GameScene.h"


GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}

Scenes GameScene::Update(double _timer)
{
	return nextScene;
}

void GameScene::Render()
{

}

bool GameScene::Load()
{
	return false;
}
