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
	void ExpensiveLoad() override;
	void ExpensiveUnload() override {};

private:
	//Create the scene's objects
	void create2DObjects() override;
	void create3DObjects() override {};
	void pushBackObjects() override {};
	void CreateCharacterMenu();
	void CreateVehiclesMenu();
	void CupSelect();
	void MapSelect();
	void VehicleSelect();
	void CharacterSelect();
	void CheckAvailabilty(int player, int& player_sel_number, int added_num, int limit, int reset, const std::vector<int>& vector_compare);
	bool NavigateMenus(int player, int& number, const std::vector<ImageGO2D*>& previews);

	//Background
	ImageGO2D* m_cupBackground = nullptr;
	ImageGO2D* m_characterBackground = nullptr;
	Text2D* m_state_desc = nullptr;

	//Main Menu select objects
	int highlighted_type = 0;
	std::vector<Text2D*> m_mainMenuTitles;

	//Cup select objects
	int highlighted_cup = 0;
	std::vector<Text2D*> m_cupTitles;
	std::vector<ImageGO2D*> m_cupPreviews;

	//Map select objects
	int highlighted_map = 0;
	std::vector<Text2D*> m_mapTitles;
	std::vector<ImageGO2D*> m_mapPreviews;

	//Character select objects
	std::vector<Text2D*> m_characterTitles;
	std::vector<ImageGO2D*> m_characterPreviews;
	std::vector<int> highlighted_character{ 0, 0, 0, 0 };
	std::vector<std::vector<ImageGO2D*>> m_selectedCharacters;
	std::vector<ImageGO2D*> m_characterHighlight;
	std::vector<ImageGO2D*> m_AButton;

	//Vehicle select objects
	std::vector<Text2D*> m_vehicleTitles;
	std::vector<ImageGO2D*> m_vehiclePreviews;
	std::vector<int> highlighted_vehicle{ 0,0,0,0 };
	std::vector<std::vector<ImageGO2D*>> m_selectedVehicles;
	std::vector<ImageGO2D*> m_vehicleHighlight;

	//Common engine components
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;
	GameFilepaths m_filepath;

	//Referenced stuff
	SceneManager* m_scene_manager;

	//Colours
	XMVECTORF32 active_colour = Colors::White;
	XMVECTORF32 inactive_colour = Colors::LightGray;

	//Timing
	float m_timeout = 3.0f;
	float m_timer = 0.0f;
	float m_idle_timer = 0.0f;
	float m_idle_timeout = 30;

	//Logo
	ImageGO2D* m_logo = nullptr;
	ImageGO2D* m_splash_bg = nullptr;

	//Configs
	json game_config;

	//States
	enum menu_states { SPLASH, MAIN_SELECT, CUP_SELECT, MAP_SELECT, CHARACTER_SELECT, VEHICLE_SELECT };
	menu_states m_menu_state = menu_states::SPLASH;

	int num_of_charcters = 0;
	bool players_joined[4]{ true, false, false, false };
	float highlight_diff;
	int m_maxPlayers = 4;
	bool menu_start = true;
};

