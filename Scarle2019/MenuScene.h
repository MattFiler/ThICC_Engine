#pragma once

#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();

	Scenes Update(GameStateData* _GSD, InputData* _ID) override;
	void Render(RenderData* _RD, WindowData* _WD) override;
	bool Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD) override;

private:

};

