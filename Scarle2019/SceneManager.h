#pragma once

#include "Scene.h"
#include "StepTimer.h"
#include "Constants.h"
#include "Constants.h"

class SceneManager
{
public:
	SceneManager();

	int Update(double _timer);
	void Render();
	bool Load();

	Scene* currScene = nullptr;

private:
	void ChangeScene(Scene* _newScene);

};

