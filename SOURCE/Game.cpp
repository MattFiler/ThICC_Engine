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
	m_scene_manager.addScene(new MenuScene(), Scenes::MENUSCENE);
	//m_scene_manager.addScene(new GameScene(game_config["default_track"]), Scenes::GAMESCENE);
	m_scene_manager.addScene(new GameScene("MARIOKARTSTADIUM"), Scenes::GAMESCENE);
	m_scene_manager.addScene(new GameScene("DRAGON_DRIFTWAY"), Scenes::DEBUGSCENE);
	//m_sceneManager.addScene(new DebugScene(), Scenes::DEBUGSCENE);

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