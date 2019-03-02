#include "pch.h"
#include "MenuScene.h"


MenuScene::MenuScene()
{
}


MenuScene::~MenuScene()
{
}

Scenes MenuScene::Update(double _timer)
{
	return nextScene;
}

void MenuScene::Render()
{

}

bool MenuScene::Load()
{
	return true;
}
