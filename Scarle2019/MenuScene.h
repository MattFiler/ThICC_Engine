#pragma once

#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();

	Scenes Update(double _timer);
	void Render();
	bool Load();

private:

};

