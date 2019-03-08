#pragma once

#include "StepTimer.h"
#include "Scene.h"
#include <vector>

using std::vector;

//predefs
class GameObject2D;
class GameObject3D;
struct RenderData;
struct InputData;

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	Scenes Update(GameStateData* _GSD, InputData* _ID) override;
	void Render(RenderData* _RD) override;
	bool Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD) override;

private:
	//State Stuff
	bool m_isPaused;

	void create2DObjects(RenderData* _RD);
	void create3DObjects(RenderData* _RD, InputData* _ID, WindowData* _WD);
	void pushBackObjects(RenderData* _RD);

	// Test objects
	Track* track = nullptr;
	Player* player[1] = { nullptr };
	PhysModel* test_model = nullptr;

	vector<GameObject2D*>								m_2DObjects; //data structure for all 2D Objects
	vector<GameObject3D*>								m_3DObjects; //data structure for all 3D Objects
	Camera*												m_cam[1];
	Light*												m_light;
	int num_of_players = 1;
	int num_of_cam = 1;

	KeybindManager m_keybinds;

	std::vector<PhysModel*> m_physModels;

	json game_config;
	GameFilepaths m_filepath;
};

