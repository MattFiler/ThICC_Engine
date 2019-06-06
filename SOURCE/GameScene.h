#pragma once

#include "StepTimer.h"
#include "Scene.h"
#include "LocalisationManager.h"
#include "ItemPools.h"
#include "Banana.h"
#include "Mushroom.h"
#include "GreenShell.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "AIScheduler.h"
#include "MapInfo.h"
#include "Skybox.h"
#include "InGameUI.h"
#include <vector>
#include <json.hpp>
using json = nlohmann::json;

class GameScene : public Scene
{
public:
	GameScene(MapInfo* _track);
	~GameScene() override;

	//Core update/render/load functions
	void Update(DX::StepTimer const& timer) override;
	void Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override;
	void render3DObjects();
	void Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override;
	bool Load() override;

	//Load/unload the expensive objects in the scene (only keep expensive stuff in memory if we're active)
	void ExpensiveLoad() override;
	void ExpensiveUnload() override;

private:
	//Handle player's waypoints and placements
	void SetPlayersWaypoint();
	void SetPlayerRanking();

	// Updates
	void UpdateItems();
	void DeleteItem(Item * item);
	void DeleteThread(Item * item);

	//State Stuff
	bool m_isPaused;

	//Item creation
	void create2DObjects() override;
	void create3DObjects() override;
	void pushBackObjects() override;

	std::vector<Player*> GetPlayers();

	//Structures for the scene's objects
	std::vector<GameObject2D*> m_2DObjects;
	std::vector<GameObject3D*> m_3DObjects;
	std::vector<PhysModel*> m_physModels;

	//Common engine components
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;
	GameFilepaths m_filepath;

	//Item objects
	Item* CreateItem(ItemType type);
	void loadItemDebugCollider(Item* item);
	Explosion* CreateExplosion(ItemType _ownerType);
	std::vector<Item*> m_itemModels;

	//The scene's track object (including submeshes)
	Track* track = nullptr;

	//Players and UI
	Player* player[12] = { nullptr }; //Max of twelve players in-game
	InGameUI* m_game_ui[4] = { nullptr }; //Max of four players on-screen
	Text2D* countdown_text = nullptr; //Shared between all players (TODO: MOVE TO InGameUI)

	//Cameras
	Camera* m_cam[4];
	Camera*	cine_cam;

	//Random data stores
	json game_config;
	json track_config;
	MapInfo* map_info;
	ItemPools* m_itemPools = nullptr;

	//Referenced stuff
	RenderData* m_RD;

	//Pause screen
	ImageGO2D* m_pause_screen = nullptr;

	//Our scene manager instance
	SceneManager* m_scene_manager;

	//Timing
	float timeout = 12.f;

	//Game states
	enum States {
		START = 0,
		OPENING = 1,
		CAM_OPEN = 2,
		COUNTDOWN = 3,
		PLAY = 4,
		END = 5
	};
	States state = START;
	bool m_playerControls = false;
	bool track_music_start = true;
	bool final_lap_start = false;
	bool final_lap = false;
	int finished = 0;
	bool is_paused = false;
	bool race_finished = false;
	float finish_timer = 0.0f;
	int m_maxPlayers = 12;
	int player_rank_save[4];

};

