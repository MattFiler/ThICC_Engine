#pragma once

#include "StepTimer.h"
#include "Scene.h"
#include "LocalisationManager.h"
#include "Banana.h" //test
#include "GreenShell.h"
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
	// Updates
	void UpdateItems();

	//State Stuff
	bool m_isPaused;

	void create2DObjects();
	void create3DObjects();
	void pushBackObjects();
	void playerControlsActive();

	Item* CreateItem(ItemType type);

	// Test objects
	Track* track = nullptr;
	Player* player[4] = { nullptr };
	PhysModel* test_model = nullptr;
	SDKMeshGO3D* debug_cups[8] = { nullptr };

	vector<GameObject2D*>								m_2DObjects; //data structure for all 2D Objects
	vector<GameObject3D*>								m_3DObjects; //data structure for all 3D Objects
	//std::ptr<DirectX::SpriteBatch> m_spriteBatch[4]{NULL};
	Camera*												m_cam[4];
	Light*												m_light;

	KeybindManager m_keybinds;
	//Banana m_banana_test;
	std::vector<Item*> m_itemModels;

	std::vector<PhysModel*> m_physModels;

	json game_config;
	LocalisationManager m_localiser;
	GameFilepaths m_filepath;
	SceneManager* m_scene_manager;
	
	RenderData* m_RD;
	float m_startTimer = 3.0f;
	bool m_playerControls = false;
};

