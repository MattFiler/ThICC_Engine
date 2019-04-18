#pragma once

#include "StepTimer.h"
#include "SceneManager.h"

class ThICC_Game {
public:
	ThICC_Game() = default;
	~ThICC_Game() = default;

	void Initialize();
	void Update(DX::StepTimer const& timer);
	void Render();

private:
	SceneManager m_scene_manager;
	json game_config;
	GameFilepaths m_filepath;
};