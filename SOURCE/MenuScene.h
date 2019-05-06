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

	//Background
	ImageGO2D* m_background = nullptr;
	Text2D* m_state_desc = nullptr;

	//Cup select objects
	int highlighted_cup = 0;
	std::vector<Text2D*> m_cupTitles;
	std::vector<ImageGO2D*> m_cupPreviews;

	//Map select objects
	int highlighted_map = 0;
	std::vector<Text2D*> m_mapTitles;
	std::vector<ImageGO2D*> m_mapPreviews;

	//Character select objects
	int highlighted_character = 0;
	std::vector<Text2D*> m_characterTitles;
	std::vector<ImageGO2D*> m_characterPreviews;

	//Vehicle select objects
	int highlighted_vehicle = 0;
	std::vector<Text2D*> m_vehicleTitles;
	std::vector<ImageGO2D*> m_vehiclePreviews;

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
	float m_timeout = 5.0f;
	float m_timer = 0.0f;

	//Logo
	ImageGO2D* m_logo = nullptr;
	ImageGO2D* m_splash_bg = nullptr;

	//Configs
	json game_config;

	//States
	enum menu_states { SPLASH, CUP_SELECT, MAP_SELECT, CHARACTER_SELECT, VEHICLE_SELECT };
	menu_states m_menu_state = menu_states::SPLASH;


	//TEST SPRITE
	ImageGO2D* GamepadTestSprite = nullptr;
};

