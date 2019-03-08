#pragma once

#include "StepTimer.h"
#include "Scene.h"
#include "LocalisationManager.h"
#include <vector>

using std::vector;

//predefs
class GameObject2D;
class GameObject3D;
struct RenderData;
struct InputData;

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene();

	Scenes Update(GameStateData* _GSD, InputData* _ID) override;
	void Render(RenderData* _RD, WindowData* _WD) override;
	bool Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD) override;

private:

	void create2DObjects(RenderData* _RD);
	void create3DObjects(RenderData* _RD, InputData* _ID, WindowData* _WD);
	void pushBackObjects(RenderData* _RD);

	vector<GameObject2D*>								m_2DObjects; //data structure for all 2D Objects
	vector<GameObject3D*>								m_3DObjects; //data structure for all 3D Objects
	Camera*												m_cam[4];
	Light*												m_light;

	int num_of_cam = 1;

	std::vector<PhysModel*> m_physModels;

	json game_config;
	LocalisationManager m_localiser;
	GameFilepaths m_filepath;
	KeybindManager m_key; 

	Text2D* m_enterMenu = new Text2D("Lewis is not cool.");
};

