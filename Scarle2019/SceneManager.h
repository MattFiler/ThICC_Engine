#pragma once

#include "Scene.h"
#include "StepTimer.h"
#include "Constants.h"

class SceneManager
{
public:
	SceneManager();

	int Update(GameStateData* _GSD, InputData* _ID);
	void Render(RenderData* _RD);

	Scene* currScene = nullptr;

private:
	void ChangeScene(Scene* _newScene);
};

