#pragma once

#include "StepTimer.h"
#include "Scene.h"
#include <vector>

using std::vector;

//predefs
class GameObject2D;
class GameObject3D;
struct RenderData;

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	Scenes Update(double _timer);
	void Render();
	bool Load();

private:

};

