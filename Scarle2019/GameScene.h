#pragma once

#include "StepTimer.h"
#include "Scene.h"
#include "LocalisationManager.h"
#include "Banana.h" //test
#include "CollisionManager.h"
#include <vector>

using std::vector;

//predefs
class GameObject2D;
class GameObject3D;
struct RenderData;
struct InputData;
class SceneManager;

class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	void Update() override;
	void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList) override;
	bool Load() override;
	void SetPlayersWaypoint();
	void SetPlayerRanking();

private:
	//State Stuff
	bool m_isPaused;

	void create2DObjects();
	void create3DObjects();
	void pushBackObjects();

	// Test objects
	Track* track = nullptr;
	Player* player[4] = { nullptr };
	PhysModel* test_model = nullptr;

	vector<GameObject2D*>								m_2DObjects; //data structure for all 2D Objects
	vector<GameObject3D*>								m_3DObjects; //data structure for all 3D Objects

	Camera*												m_cam[4];
	Camera*												cine_cam;
	Light*												m_light;

	KeybindManager m_keybinds;
	//Banana m_banana_test;

	std::vector<PhysModel*> m_physModels;

	json game_config;
	LocalisationManager m_localiser;
	GameFilepaths m_filepath;
	SceneManager* m_scene_manager;

	RenderData* m_RD;

	// useful debug code dont delete
	Text2D* camera_pos = nullptr;
	Text2D* countdown_text = nullptr;

	float timeout = 8.7f;

	enum States {
		OPENING = 0,
		COUNTDOWN = 1,
		PLAY = 2,
		END = 3
	};

	States state = OPENING;

};

