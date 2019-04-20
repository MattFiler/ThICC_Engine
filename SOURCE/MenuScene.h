#pragma once

#include "pch.h"
#include "Camera.h"
#include "StepTimer.h"
#include "Scene.h"
#include "PhysModel.h"
#include "ImageGO2D.h"
#include "Text2D.h"
#include "LocalisationManager.h"
#include <vector>

class MenuScene : public Scene
{
public:
	MenuScene();
	~MenuScene() override;

	//Core update/render/load functions
	void Update(DX::StepTimer const& timer) override;
	void Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override {};
	void Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) override;
	bool Load() override;

	//Load/unload the expensive objects in the scene (only keep expensive stuff in memory if we're active)
	void ExpensiveLoad() override {};
	void ExpensiveUnload() override {};

private:
	//Create the scene's objects
	void create2DObjects() override;
	void create3DObjects() override {};
	void pushBackObjects() override {};

	//Handle players in the "lobby"
	void enterPlayerLobby();
	void playerJoin();
	void initCharacterImages();
	void resetCharacterImagePos();

	//Structures for the scene's objects
	std::vector<GameObject2D*> m_2DObjects;

	//Common engine components
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;
	GameFilepaths m_filepath;

	//Referenced stuff
	SceneManager* m_scene_manager;

	//Charecter Images
	ImageGO2D* m_character_images[4][4]; //[player][character]

	//Configs
	json game_config;

	//States and timings
	float m_charTimeout[4] = { 0.0f,0.0f,0.0f,0.0f };
	float timeout = 1.2f;
	bool intro_music_start = true;
	bool game_start = true;
	States m_menu_state = States::NOSTATE;
};

