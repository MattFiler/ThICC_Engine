#pragma once

#include "StepTimer.h"
#include "SceneManager.h"
#include "LocalisationManager.h"
#include "ImageGO2D.h"
#include "MapInfo.h"
#include <vector>

class ThICC_Game {
public:
	ThICC_Game() = default;
	~ThICC_Game() = default;

	void Initialize();
	void Update(DX::StepTimer const& timer);
	void Render();

private:
	SceneManager m_scene_manager;
	GameFilepaths m_filepath;
	LocalisationManager m_localiser;

	json game_config;
	json map_config;
	std::vector<MapInfo> map_instances;
};