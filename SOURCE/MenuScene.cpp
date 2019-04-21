#include "pch.h"
#include "MenuScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "Text2D.h"
#include "GameObjectShared.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();

/* Create! */
MenuScene::MenuScene()
{
	//Get a ref to the scene manager for swapping scenes
	m_scene_manager = Locator::getSM();
}

/* Destroy! */
MenuScene::~MenuScene()
{
	m_2DObjects.clear();
	m_mapSelections.clear();
}

/* Load inexpensive things and create the objects for expensive things we will populate when required */
bool MenuScene::Load()
{
	create2DObjects();
	return true;
}

/* Create all 2D objects for the scene */
void MenuScene::create2DObjects()
{
	//ImageGO2D* splash_screen = new ImageGO2D("MENU_SPLASHSCREEN");
	//m_2DObjects.push_back(splash_screen);

	//position map options
	int index = 0;
	for (MapInfo a_map : Locator::getGOS()->map_instances) {
		index++;
		Text2D* map_name = new Text2D(a_map.name);
		if (index == 1) {
			map_name->SetText("> " + map_name->GetText());
		}
		map_name->SetColour(Colors::Black);
		map_name->SetPos(Vector2(Locator::getRD()->m_window_width/2, index * 40));
		m_mapSelections.push_back(map_name);
	}
}

/* Update the scene */
void MenuScene::Update(DX::StepTimer const& timer)
{
	//Exit
	if (m_keybinds.keyPressed("Quit"))
	{
		ExitGame();
	}

	//Change map selection
	if (m_keybinds.keyPressed("Down Arrow"))
	{
		if (highlighted_map < m_mapSelections.size()-1) {
			m_mapSelections.at(highlighted_map)->SetText(m_mapSelections.at(highlighted_map)->GetText().substr(2));
			highlighted_map++;
			m_mapSelections.at(highlighted_map)->SetText("> " + m_mapSelections.at(highlighted_map)->GetText());
		}
	}
	if (m_keybinds.keyPressed("Up Arrow"))
	{
		if (highlighted_map > 0) {
			m_mapSelections.at(highlighted_map)->SetText(m_mapSelections.at(highlighted_map)->GetText().substr(2));
			highlighted_map--;
			m_mapSelections.at(highlighted_map)->SetText("> " + m_mapSelections.at(highlighted_map)->GetText());
		}
	}

	//Images
	for (std::vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick();
	}

	//Load selected map
	if (m_keybinds.keyPressed("Activate"))
	{
		Locator::getAudio()->GetSound(SOUND_TYPE::MENU, (int)SOUNDS_MENU::MENU_LOOP)->Stop();
		m_scene_manager->setCurrentScene(Scenes::GAMESCENE+highlighted_map);
	}
}

/* Render the 2D scene */
void MenuScene::Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	for (std::vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Render();
	}

	for (std::vector<Text2D*>::iterator it = m_mapSelections.begin(); it != m_mapSelections.end(); it++) {
		(*it)->Render();
	}
}
