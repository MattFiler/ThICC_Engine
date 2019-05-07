#pragma once

#include "StepTimer.h"
#include "SceneManager.h"
#include "LocalisationManager.h"
#include "ImageGO2D.h"
#include "AssetCompPch.h"
#include "GameObjectShared.h"
#include <vector>

class ThICC_Game {
public:
	ThICC_Game() = default;
	~ThICC_Game() = default;

	void Initialize();
	void Render3D();
	void Render2D();
	void Update(DX::StepTimer const& timer);

private:
	SceneManager m_scene_manager;
	GameFilepaths m_filepath;
	LocalisationManager m_localiser;

	json game_config;
	json map_config;
	json character_config;
	json cup_config;
	json vehicle_config;

	GameObjectShared m_go_shared;

	std::unique_ptr<AIScheduler> m_aiScheduler = nullptr;
};