#include "pch.h"
#include "Game.h"
#include "RenderData.h"

#include "MenuScene.h"
#include "GameScene.h"

/* Perform initialisation */
void ThICC_Game::Initialize() {
	m_scene_manager.addScene(new MenuScene(), Scenes::MENUSCENE);
	//m_scene_manager.addScene(new GameScene(), Scenes::GAMESCENE);
	//m_sceneManager.addScene(new DebugScene(), Scenes::DEBUGSCENE);
	m_scene_manager.setCurrentScene(Scenes::MENUSCENE);
}

/* Update loop */
void ThICC_Game::Update(DX::StepTimer const& timer) {

	m_scene_manager.Update();
}

/* Render loop */
void ThICC_Game::Render() {
	//m_scene_manager.Render(Locator::getRD()->m_commandList);
}