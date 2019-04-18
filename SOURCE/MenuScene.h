#pragma once

#include "pch.h"
#include "Camera.h"
#include "StepTimer.h"
#include "Scene.h"
#include "PhysModel.h"
#include "ImageGO2D.h"
#include "LocalisationManager.h"
#include <vector>

using std::vector;

//predefs
class GameObject2D;
class GameObject3D;
struct RenderData;
struct InputData;
class SceneManager;

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene() override;

	//Core update/render/load functions
	void Update() override;
	void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override;
	bool Load() override;

	//Load/unload the expensive objects in the scene (only keep expensive stuff in memory if we're active)
	void ExpensiveLoad() override;
	void ExpensiveUnload() override;

private:
	//Create the scene's objects
	void create2DObjects() override;
	void create3DObjects() override;
	void pushBackObjects() override;

	//Handle players in the "lobby"
	void enterPlayerLobby();
	void playerJoin();
	void initCharecterImages();
	void resetCharecterImagePos();

	//Structures for the scene's objects
	vector<GameObject2D*> m_2DObjects;
	vector<GameObject3D*> m_3DObjects;
	std::vector<PhysModel*> m_physModels;

	//Common engine components
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;
	GameFilepaths m_filepath;

	//The scene's camera
	Camera* m_cam;
	int num_of_cam = 1;

	//Referenced stuff
	json game_config;

	//lol wtf
	SceneManager* m_scene_manager;

	//Charecter Images
	ImageGO2D* m_charecter_images[4][4]; //[player][charecter]

	//States and timings
	float m_charTimeout[4] = { 0.0f,0.0f,0.0f,0.0f };
	float timeout = 1.2f;
	bool intro_music_start = true;
	bool game_start = true;
	States m_menu_state = States::NOSTATE;
};

