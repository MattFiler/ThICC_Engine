#include "pch.h"
#include "MenuScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "RaceManager.h"
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
	m_mapTitles.clear();
	m_mapPreviews.clear();
	m_characterTitles.clear();
	m_characterPreviews.clear();
	m_selectedCharacters.clear();
	m_selectedVehicles.clear();
	highlighted_character.clear();
	m_selectedVehicles.clear();
	m_characterHighlight.clear();
	m_vehicleHighlight.clear();
	delete m_cupBackground;
	delete m_characterBackground;
}

/* Load inexpensive things and create the objects for expensive things we will populate when required */
bool MenuScene::Load()
{
	create2DObjects();
	return true;
}

/* Reset on load */
void MenuScene::ExpensiveLoad() {
	Locator::getRM()->attract_state = false;
	m_menu_state = menu_states::SPLASH;
	m_timer = 0.0f;
	m_idle_timer = 0.0f;
	m_keybinds.Reset();
	highlighted_cup = 0;
	highlighted_map = 0;
	highlighted_type = 0;
	m_maxPlayers = 4;
	for (size_t i = 0; i < 4; i++)
	{
		if (i != 0)
			players_joined[i] = false;

		highlighted_character[i] = 0;
		highlighted_vehicle[i] = 0;
	}
}

/* Create all 2D objects for the scene */
void MenuScene::create2DObjects()
{
	//Splashscreen objects
	m_splash_bg = new ImageGO2D("WHITE_720");
	m_logo = new ImageGO2D("engine_logo_white_720");
	m_logo->SetScale(Vector2(0.3, 0.3));
	m_logo->SetPos(Vector2(Locator::getRD()->m_window_width / 2.f, Locator::getRD()->m_window_height / 2.f), false);
	m_logo->CentreOrigin();

	//Main menu objects
	m_cupBackground = new ImageGO2D("MAIN_MENU_TEMP");
	m_state_desc = new Text2D("", Text2D::MIDDLE);
	m_state_desc->SetPos(Vector2(498, 620));
	m_state_desc->SetColour(Colors::Black);
	m_state_desc->SetScale(0.5f);
	m_characterBackground = new ImageGO2D("CHARACTER_SELECT");
	m_state_desc = new Text2D("", Text2D::MIDDLE);
	m_state_desc->SetPos(Vector2(498, 620));
	m_state_desc->SetColour(Colors::Black);
	m_state_desc->SetScale(0.5f);

	Text2D* cup_text = new Text2D(m_localiser.getString("CUP"));
	cup_text->SetPos(Vector2(209, 55 + 47 * 1));
	m_mainMenuTitles.push_back(cup_text);
	Text2D* race_text = new Text2D(m_localiser.getString("RACE"));
	race_text->SetPos(Vector2(209, 55 + 47 * 2));
	race_text->SetColour(inactive_colour);
	m_mainMenuTitles.push_back(race_text);
	Text2D* exit_text = new Text2D(m_localiser.getString("EXIT"));
	exit_text->SetPos(Vector2(209, 55 + 47 * 3));
	exit_text->SetColour(inactive_colour);
	m_mainMenuTitles.push_back(exit_text);

	//position cup options
	int index = 0;
	for (CupInfo* a_cup : Locator::getGOS()->cup_instances) {
		index++;

		//Text
		Text2D* cup_name = new Text2D(a_cup->name);
		cup_name->SetColour(inactive_colour);
		if (index == 1) {
			cup_name->SetColour(active_colour);
		}
		cup_name->SetPos(Vector2(209, 55 + (index * 47)));
		m_cupTitles.push_back(cup_name);

		//Image
		a_cup->preview_sprite->SetPos(Vector2(812, 279));
		m_cupPreviews.push_back(a_cup->preview_sprite);
	}

	CreateCharacterMenu();
	CreateVehiclesMenu();

	//position character options
	for (size_t i = 0; i < m_maxPlayers; ++i) {

		ImageGO2D* image = m_keybinds.getInputIcon("PAUSE");

		ImageGO2D* character_highlight = new ImageGO2D("PLAYER_" + std::to_string(i + 1) + "_HIGHLIGHT");
		ImageGO2D* vehicle_highlight = new ImageGO2D("PLAYER_" + std::to_string(i + 1) + "_HIGHLIGHT");

		character_highlight->SetSize({ m_characterPreviews[0]->GetSize().x / 0.9375f, m_characterPreviews[0]->GetSize().y / 0.9375f });
		vehicle_highlight->SetSize({ m_characterPreviews[0]->GetSize().x / 0.9375f, m_characterPreviews[0]->GetSize().y / 0.9375f });

		if (i == 0)
			image->SetPos(Vector2(160, 100));
		else if (i == 1)
			image->SetPos(Vector2(970, 100));
		else if (i == 2)
			image->SetPos(Vector2(160, 420));
		else if (i == 3)
			image->SetPos(Vector2(970, 420));

		image->SetSize({ 150, 150 });

		//Image
		m_AButton.push_back(image);
		m_characterHighlight.push_back(character_highlight);
		m_vehicleHighlight.push_back(vehicle_highlight);
	}
	highlight_diff = (m_vehicleHighlight[0]->GetSize().x - m_characterPreviews[0]->GetSize().x) * 0.5f;
}

void MenuScene::CreateCharacterMenu()
{
	//position character options
	int index = 0;
	int x_pos = 340;
	int y_pos = 55 + 47;
	std::vector<ImageGO2D*> characterImages1;
	std::vector<ImageGO2D*> characterImages2;
	std::vector<ImageGO2D*> characterImages3;
	std::vector<ImageGO2D*> characterImages4;
	for (CharacterInfo* a_character : Locator::getGOS()->character_instances) {
		index++;

		a_character->preview_sprite->SetPos(Vector2(x_pos, y_pos));
		a_character->preview_sprite->SetSize({ 140, 140 });
		x_pos += a_character->preview_sprite->GetSize().x + 10;

		if (index % 4 == 0 && index != 0)
		{
			y_pos += a_character->preview_sprite->GetSize().y + 10;
			x_pos = 340;
		}
		//Image
		m_characterPreviews.push_back(a_character->preview_sprite);

		for (int i = 0; i < 4; i++)
		{
			ImageGO2D* image = new ImageGO2D(a_character->preview_sprite->GetFilepath());
			image->SetSize(Vector2(150, 150));
			if (i == 0)
			{
				image->SetPos(Vector2(160, 100));
				characterImages1.push_back(image);
			}
			else if (i == 1)
			{
				image->SetPos(Vector2(970, 100));
				characterImages2.push_back(image);
			}
			else if (i == 2)
			{
				image->SetPos(Vector2(160, 420));
				characterImages3.push_back(image);
			}
			else if (i == 3)
			{
				image->SetPos(Vector2(970, 420));
				characterImages4.push_back(image);
			}
		}
	}
	m_selectedCharacters.push_back(characterImages1);
	m_selectedCharacters.push_back(characterImages2);
	m_selectedCharacters.push_back(characterImages3);
	m_selectedCharacters.push_back(characterImages4);
	num_of_charcters = index;
}

void MenuScene::CreateVehiclesMenu()
{
	//position vehicle options
	int index = 0;
	int x_pos = 340;
	int y_pos = 55 + 47;
	std::vector<ImageGO2D*> vehiclesImages1;
	std::vector<ImageGO2D*> vehiclesImages2;
	std::vector<ImageGO2D*> vehiclesImages3;
	std::vector<ImageGO2D*> vehiclesImages4;
	for (VehicleInfo* a_vehicle : Locator::getGOS()->vehicle_instances) {
		index++;

		//Image
		a_vehicle->preview_sprite->SetPos(Vector2(x_pos, y_pos));
		a_vehicle->preview_sprite->SetSize({ 140, 140 });
		m_vehiclePreviews.push_back(a_vehicle->preview_sprite);
		x_pos += a_vehicle->preview_sprite->GetSize().x + 10;

		if (index % 4 == 0 && index != 0)
		{
			y_pos += a_vehicle->preview_sprite->GetSize().y + 10;
			x_pos = 340;
		}
		for (int i = 0; i < 4; i++)
		{
			ImageGO2D* image = new ImageGO2D(a_vehicle->preview_sprite->GetFilepath());
			image->SetSize({ 150, 150 });
			if (i == 0)
			{
				image->SetPos(Vector2(160, 100));
				vehiclesImages1.push_back(image);
			}
			else if (i == 1)
			{
				image->SetPos(Vector2(970, 100));
				vehiclesImages2.push_back(image);
			}
			else if (i == 2)
			{
				image->SetPos(Vector2(160, 420));
				vehiclesImages3.push_back(image);
			}
			else if (i == 3)
			{
				image->SetPos(Vector2(970, 420));
				vehiclesImages4.push_back(image);
			}
		}
	}
	m_selectedVehicles.push_back(vehiclesImages1);
	m_selectedVehicles.push_back(vehiclesImages2);
	m_selectedVehicles.push_back(vehiclesImages3);
	m_selectedVehicles.push_back(vehiclesImages4);
}

/* Update the scene */
void MenuScene::Update(DX::StepTimer const& timer)
{
#ifdef _DEBUG
	//Hacky implementation to get to the debug scene (temp)
	if (Locator::getID()->m_keyboardTracker.IsKeyReleased(DirectX::Keyboard::Keys::D))
	{
		m_scene_manager->setCurrentScene(Scenes::DEBUG_LIGHTINGTEST);
	}
#endif

#ifdef _ARCADE
	//Arcade loads straight to an arcade map
	Locator::getGSD()->character_selected[0] = m_characterHighlightInt[0]; =
		Locator::getGSD()->vehicle_selected[0] = m_vehicleHighlightInt[0];
	Locator::getRM()->player_amount = 1;
	Locator::getRM()->is_cup = false;
	int index = 0;
	for (MapInfo* a_map : Locator::getGOS()->map_instances) {
		if (a_map->is_arcade_exclusive) {
			m_scene_manager->setCurrentScene(Scenes::GAMESCENE + index);
			return;
		}
		index++;
	}
#endif	 

	//If idle for too long, send to attraction state
	m_idle_timer += (float)timer.GetElapsedSeconds();
	if (m_idle_timer > m_idle_timeout) {
		Locator::getRM()->attract_state = true;
		Locator::getRM()->player_amount = 1;
		m_scene_manager->setCurrentScene(Scenes::ATTRACT_STATE);
	}

	switch (m_menu_state) {
	case menu_states::SPLASH:
		//Animate logo over time
		m_logo->SetScale(Vector2(0.3 + (m_timer / 30), 0.3 + (m_timer / 30)));
		m_timer += (float)timer.GetElapsedSeconds();

		//Allow skip or progress if time elapsed
		if (m_keybinds.keyReleased("Activate") || m_timer > m_timeout)
		{
			m_menu_state = menu_states::MAIN_SELECT;
			m_idle_timer = 0.0f;
		}

		break;
	case menu_states::MAIN_SELECT:
		m_state_desc->SetText(m_localiser.getString("main_select"));
		if (m_keybinds.keyReleased("Menu Down") || m_keybinds.keyReleased("backwards"))
		{
			if (highlighted_type < m_mainMenuTitles.size() - 1) {
				m_mainMenuTitles.at(highlighted_type)->SetColour(inactive_colour);
				highlighted_type++;
				m_mainMenuTitles.at(highlighted_type)->SetColour(active_colour);
			}
		}
		else if (m_keybinds.keyReleased("Menu Up") || m_keybinds.keyReleased("forward"))
		{
			if (highlighted_type > 0) {
				m_mainMenuTitles.at(highlighted_type)->SetColour(inactive_colour);
				highlighted_type--;
				m_mainMenuTitles.at(highlighted_type)->SetColour(active_colour);
			}
		}
		else if (m_keybinds.keyReleased("Activate"))
		{
			m_idle_timer = 0.0f;
			for (size_t i = 0; i < m_mainMenuTitles.size(); i++)
			{
				m_mainMenuTitles.at(i)->SetColour(i == 0 ? active_colour : inactive_colour);
			}
			//Swap scene
			m_menu_state = menu_states::CUP_SELECT;
			if (m_mainMenuTitles[highlighted_type]->GetText() == m_localiser.getString("CUP"))
				Locator::getRM()->is_cup = true;
			else if (m_mainMenuTitles[highlighted_type]->GetText() == m_localiser.getString("RACE"))
				Locator::getRM()->is_cup = false;
			else if (m_mainMenuTitles[highlighted_type]->GetText() == m_localiser.getString("EXIT"))
			{
				ExitGame();
				m_menu_state = menu_states::SPLASH;
			}
		}
		break;
	case menu_states::CUP_SELECT:
		CupSelect();
		break;
	case menu_states::MAP_SELECT:
		MapSelect();
		break;
	case menu_states::CHARACTER_SELECT:
		CharacterSelect();
		break;
	case menu_states::VEHICLE_SELECT:
		VehicleSelect();
		break;
	}

	for (int i = 0; i < m_maxPlayers; ++i)
	{
		m_vehicleHighlight[i]->SetPos(m_vehiclePreviews[highlighted_vehicle[i]]->GetPos() - Vector2{ highlight_diff, highlight_diff });
		m_characterHighlight[i]->SetPos(m_characterPreviews[highlighted_character[i]]->GetPos() - Vector2{ highlight_diff, highlight_diff });
	}
}

/* Cup selection */
void MenuScene::CupSelect()
{
	m_state_desc->SetText(m_localiser.getString("cup_select"));

	//Exit
	if (m_keybinds.keyReleased("Back")) {
		m_menu_state = menu_states::MAIN_SELECT;
		highlighted_type = 0;
		m_idle_timer = 0.0f;
	}

	//Change cup selection
	if (m_keybinds.keyReleased("Menu Down") || m_keybinds.keyReleased("backwards"))
	{
		if (highlighted_cup < m_cupTitles.size() - 1) {
			m_cupTitles.at(highlighted_cup)->SetColour(inactive_colour);
			highlighted_cup++;
			m_cupTitles.at(highlighted_cup)->SetColour(active_colour);
		}
	}
	else if (m_keybinds.keyReleased("Menu Up") || m_keybinds.keyReleased("forward"))
	{
		if (highlighted_cup > 0) {
			m_cupTitles.at(highlighted_cup)->SetColour(inactive_colour);
			highlighted_cup--;
			m_cupTitles.at(highlighted_cup)->SetColour(active_colour);
		}
	}

	//	Change to map select
	if (m_keybinds.keyReleased("Activate")) {
		m_idle_timer = 0.0f;
		if (!Locator::getRM()->is_cup)
		{
			m_mapTitles.clear();
			m_mapPreviews.clear();

			//position map options
			int index = 0;
			for (MapInfo* a_map : Locator::getGOS()->map_instances) {
				//Only include maps from this cup
				if (a_map->cup->codename != Locator::getGOS()->cup_instances.at(highlighted_cup)->codename) {
					continue;
				}

#ifndef _DEBUG
				//Show/hide map options based on arcade exclusivity
#ifdef _ARCADE
				if (!a_map->is_arcade_exclusive) {
					continue;
				}
#else
				if (a_map->is_arcade_exclusive) {
					continue;
				}
#endif
#endif

				index++;

				//Text
				Text2D* map_name = new Text2D(a_map->name);
				map_name->SetColour(inactive_colour);
				if (index == 1) {
					map_name->SetColour(active_colour);
				}
				map_name->SetPos(Vector2(209, 55 + (index * 47)));
				m_mapTitles.push_back(map_name);

				//Image
				a_map->preview_sprite->SetPos(Vector2(812, 279));
				m_mapPreviews.push_back(a_map->preview_sprite);
			}

			highlighted_map = 0;
			m_menu_state = menu_states::MAP_SELECT;
		}
		else
		{
			m_menu_state = menu_states::CHARACTER_SELECT;
		}
	}
}

/* Map selection */
void MenuScene::MapSelect()
{
	m_state_desc->SetText(m_localiser.getString("map_select"));

	//Back to cup select
	if (m_keybinds.keyReleased("Back"))
	{
		m_idle_timer = 0.0f;
		m_menu_state = menu_states::CUP_SELECT;
		highlighted_cup = 0;
	}

	//Change map selection
	if (m_keybinds.keyReleased("Menu Down") || m_keybinds.keyReleased("backwards"))
	{
		if (highlighted_map < m_mapTitles.size() - 1) {
			m_mapTitles.at(highlighted_map)->SetColour(inactive_colour);
			highlighted_map++;
			m_mapTitles.at(highlighted_map)->SetColour(active_colour);
		}
	}
	if (m_keybinds.keyReleased("Menu Up") || m_keybinds.keyReleased("forward"))
	{
		if (highlighted_map > 0) {
			m_mapTitles.at(highlighted_map)->SetColour(inactive_colour);
			highlighted_map--;
			m_mapTitles.at(highlighted_map)->SetColour(active_colour);
		}
	}

	//Change to character select
	if (m_keybinds.keyReleased("Activate"))
	{
		m_idle_timer = 0.0f;
		m_menu_state = menu_states::CHARACTER_SELECT;
	}
}

/* Character selection */
void MenuScene::CharacterSelect()
{
	m_state_desc->SetText(m_localiser.getString("character_select"));

	for (size_t i = 0; i < m_maxPlayers; ++i)
	{
		if (m_keybinds.keyReleased("Menu Right", i) || m_keybinds.keyReleased("Right", i) && players_joined[i])
		{
			CheckAvailabilty(i, highlighted_character[i], 1, num_of_charcters, 0, highlighted_character);
		}
		else if (m_keybinds.keyReleased("Menu Left", i) || m_keybinds.keyReleased("Left", i) && players_joined[i])
		{
			CheckAvailabilty(i, highlighted_character[i], -1, -1, num_of_charcters - 1, highlighted_character);
		}
		else if (!NavigateMenus(i, highlighted_character[i], m_characterPreviews) && m_keybinds.keyReleased("Pause", i))
		{
			if (!players_joined[i])
			{
				players_joined[i] = true;
				Locator::getRM()->player_amount += 1;
				Locator::getRM()->player_amount_changed = true;
				CheckAvailabilty(i, highlighted_character[i], 1, num_of_charcters, 0, highlighted_character);
			}
		}
		else if (i != 0 && m_keybinds.keyReleased("Back", i))
		{
			if (players_joined[i])
			{
				players_joined[i] = false;
				Locator::getRM()->player_amount = -1;
				Locator::getRM()->player_amount_changed = true;
			}
		}
	}

	if (m_keybinds.keyReleased("Activate", 0))
	{
		m_idle_timer = 0.0f;
		m_menu_state = menu_states::VEHICLE_SELECT;
	}
	//Back to main menu select
	else if (m_keybinds.keyReleased("Back"))
	{
		m_idle_timer = 0.0f;
		m_menu_state = menu_states::MAIN_SELECT;
		Locator::getRM()->is_cup = false;
		highlighted_cup = 0;
		highlighted_map = 0;
		highlighted_type = 0;
		Locator::getRM()->player_amount = 1;
		for (size_t i = 0; i < m_maxPlayers; ++i)
		{
			if (i != 0)
			{
				players_joined[i] = false;
			}

			highlighted_character[i] = 0;
			highlighted_vehicle[i] = 0;
		}

	}
}

/* Vehicle selection */
void MenuScene::VehicleSelect()
{
	m_state_desc->SetText(m_localiser.getString("vehicle_select"));

	//Back to character select
	if (m_keybinds.keyReleased("Back"))
	{
		m_idle_timer = 0.0f;
		m_menu_state = menu_states::CHARACTER_SELECT;
	}

	for (size_t i = 0; i < m_maxPlayers; ++i)
	{
		NavigateMenus(i, highlighted_vehicle[i], m_vehiclePreviews);
	}

	//Load selected map with character choices
	if (m_keybinds.keyReleased("Activate"))
	{
		m_idle_timer = 0.0f;
		for (size_t i = 0; i < Locator::getRM()->player_amount; i++)
		{
			Locator::getGSD()->character_selected[i] = highlighted_character[i];
			Locator::getGSD()->vehicle_selected[i] = highlighted_vehicle[i];
		}

		if (Locator::getRM()->is_cup)
		{
			int index = 0;
			for (MapInfo* a_map : Locator::getGOS()->map_instances) {
				//Only include maps from this cup
				if (a_map->cup->codename != Locator::getGOS()->cup_instances.at(highlighted_cup)->codename) {
					continue;
				}
				index++;
				Locator::getRM()->GetMaps().push_back(a_map->scene_index);
			}
			Locator::getRM()->SetRaceAmount(index);
			m_scene_manager->setCurrentScene(Scenes::GAMESCENE + Locator::getRM()->GetMaps()[Locator::getRM()->current_race_number]);
		}
		else
		{
			m_scene_manager->setCurrentScene(Scenes::GAMESCENE + highlighted_map);
		}
	}
}

/* Navigate character/vehicle selection */
bool MenuScene::NavigateMenus(int player, int& number, const std::vector<ImageGO2D*>& previews)
{
	if ((m_keybinds.keyReleased("Right", player) || m_keybinds.keyReleased("Menu_Right", player)) && players_joined[player])
	{
		number++;
		if (number >= previews.size())
		{
			number = 0;
		}
		return true;
	}
	else if ((m_keybinds.keyReleased("Left", player) || m_keybinds.keyReleased("Menu_Left", player)) && players_joined[player])
	{
		number--;
		if (number < 0)
		{
			number = previews.size() - 1;
		}
		return true;
	}
	else if ((m_keybinds.keyReleased("Backwards", player) || m_keybinds.keyReleased("Menu_Down", player)) && players_joined[player])
	{
		number += 4;
		if (number >= previews.size())
		{
			int diff = number - previews.size();
			number = 0 + diff;
			if (number >= previews.size())
			{
				number = previews.size() - 1;
			}
		}
		return true;
	}
	else if ((m_keybinds.keyReleased("Forward", player) || m_keybinds.keyReleased("Menu_Up", player)) && players_joined[player])
	{
		number -= 4;
		if (number < 0)
		{
			int diff = 0 - number;
			number = previews.size() - diff;
			if (number < 0)
			{
				number = 0;
			}
		}
		return true;
	}
	return false;
}

/* Check character/vehicle availability */
void MenuScene::CheckAvailabilty(int player, int& player_sel_number, int added_num, int limit, int reset, const std::vector<int>& vector_compare)
{
	bool checked;

	do
	{
		checked = true;

		player_sel_number += added_num;
		if (player_sel_number == limit)
			player_sel_number = reset;

		for (int j = 0; j < 4; ++j)
		{
			if (player != j && player_sel_number == vector_compare[j] && players_joined[j])
			{
				checked = false;
			}
		}

	} while (!checked);

	if (player_sel_number < 0)
	{
		player_sel_number = num_of_charcters - 1;
	}
	else if (player_sel_number >= num_of_charcters)
	{
		player_sel_number = 0;
	}

}

/* Render the 2D scene */
void MenuScene::Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	if (m_menu_state == menu_states::SPLASH) {
		m_splash_bg->Render();
		m_logo->Render();
		return;
	}

	switch (m_menu_state) {
	case menu_states::MAIN_SELECT:
		m_cupBackground->Render();
		m_cupPreviews.at(highlighted_cup)->Render();
		for (std::vector<Text2D*>::iterator it = m_mainMenuTitles.begin(); it != m_mainMenuTitles.end(); it++) {
			(*it)->Render();
		}
		break;
	case menu_states::CUP_SELECT:
		m_cupBackground->Render();
		m_cupPreviews.at(highlighted_cup)->Render();
		for (std::vector<Text2D*>::iterator it = m_cupTitles.begin(); it != m_cupTitles.end(); it++) {
			(*it)->Render();
		}
		break;
	case menu_states::MAP_SELECT:
		m_cupBackground->Render();
		m_mapPreviews.at(highlighted_map)->Render();
		for (std::vector<Text2D*>::iterator it = m_mapTitles.begin(); it != m_mapTitles.end(); it++) {
			(*it)->Render();
		}
		break;
	case menu_states::CHARACTER_SELECT:
		m_characterBackground->Render();
		for (std::vector<ImageGO2D*>::iterator it = m_characterPreviews.begin(); it != m_characterPreviews.end(); it++) {
			(*it)->Render();
		}
		for (int i = 0; i < m_maxPlayers; ++i)
		{
			if (players_joined[i])
			{
				m_characterHighlight[i]->Render();
				m_selectedCharacters[i][highlighted_character[i]]->Render();
			}
			else if (!players_joined[i])
			{
				m_AButton[i]->Render();
			}
		}
		break;
	case menu_states::VEHICLE_SELECT:
		m_characterBackground->Render();
		for (std::vector<ImageGO2D*>::iterator it = m_vehiclePreviews.begin(); it != m_vehiclePreviews.end(); it++) {
			(*it)->Render();
		}
		for (int i = 0; i < m_maxPlayers; ++i)
		{
			if (players_joined[i])
			{
				m_vehicleHighlight[i]->Render();
				m_selectedVehicles[i][highlighted_vehicle[i]]->Render();
			}
		}
		break;
	}
	m_state_desc->Render();
}
