#pragma once
#include "StepTimer.h"
#include "Constants.h"

#include <vector>

class Scene
{

public:
	Scene() {};
	~Scene() {};

	virtual Scenes Update(double _timer) = 0;
	virtual void Render() = 0;

	//Loads all the data needed for the current scene.
	virtual bool Load() = 0;

protected:
	Scenes nextScene = Scenes::NONE;

};