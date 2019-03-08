#include "pch.h"
#include "MenuScene.h"


MenuScene::MenuScene()
{
}


MenuScene::~MenuScene()
{
}

Scenes MenuScene::Update(GameStateData* _GSD, InputData* _ID)
{
	return nextScene;
}

void MenuScene::Render(RenderData* _RD)
{

}

bool MenuScene::Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	return true;
}
