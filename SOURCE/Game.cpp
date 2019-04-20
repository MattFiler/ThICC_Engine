#include "pch.h"
#include "Game.h"
#include "RenderData.h"

#include "MenuScene.h"
#include "GameScene.h"

/* Perform initialisation */
void ThICC_Game::Initialize() {
	//Read in game config - this could really do with being passed out to everything rather than being loaded over and over!
	std::ifstream i(m_filepath.generateFilepath("GAME_CORE", m_filepath.CONFIG));
	game_config << i;

	//Send out scene manager instance for scenes to grab
	Locator::setupSM(&m_scene_manager);

	//Create the scenes
	m_scene_manager.addScene(new MenuScene(), (int)Scenes::MENUSCENE);

	//Load all character data
	std::ifstream p(m_filepath.generateFilepath("CHARACTER_CONFIG", m_filepath.CONFIG));
	character_config << p;
	int index = 0;
	for (auto& element : character_config) {
		character_instances.emplace_back(element);
		index++;
	}

	//Load all vehicle data
	std::ifstream k(m_filepath.generateFilepath("VEHICLE_CONFIG", m_filepath.CONFIG));
	vehicle_config << k;
	index = 0;
	for (auto& element : vehicle_config) {
		vehicle_instances.emplace_back(element);
		index++;
	}

	//Create all game scenes (all maps) and store map preview data
	std::ifstream j(m_filepath.generateFilepath("MAP_CONFIG", m_filepath.CONFIG));
	map_config << j;
	index = 0;
	for (auto& element : map_config) {
		//Store map info
		map_instances.emplace_back(element, index);
		//Create scene
		m_scene_manager.addScene(new GameScene(map_instances.at(index)), (int)Scenes::GAMESCENE + index);
		index++;
	}

	//Set our default scene
	m_scene_manager.setCurrentScene(Scenes::MENUSCENE, true);
}

/* Update loop */
void ThICC_Game::Update(DX::StepTimer const& timer) {
	m_scene_manager.Update();
}

/* Render loop */
void ThICC_Game::Render() {
	m_scene_manager.Render(Locator::getRD()->m_commandList);
}