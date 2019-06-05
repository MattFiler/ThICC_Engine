#include "pch.h"
#include "InGameUI.h"
#include "ImageGO2D.h"
#include "Text2D.h"
#include "Player.h"
#include <fstream>

/* Load config */
InGameUI::InGameUI(int id, Vector2 size, Vector2 offset)
{
	//Load UI config
	std::ifstream i(m_filepath.generateConfigFilepath("INGAME_UI", m_filepath.CONFIG));
	config << i;
	ui_id = id;

	//Save size/offset info for splitscreen support
	ui_size = size;
	ui_offset = offset;
}

/* Unload all UI sprites */
InGameUI::~InGameUI()
{
	ExpensiveUnload();
}

/* Load in UI sprites */
void InGameUI::ExpensiveLoad()
{
	//Load in UI: course intro overlay & text
	intro_ui_sprite = new ImageGO2D(config["INTRO_OVERLAY"]["sprite"]);
	intro_ui_sprite->SetPos(Vector2(config["INTRO_OVERLAY"]["position"][0], config["INTRO_OVERLAY"]["position"][1]));
	intro_ui_text = new Text2D("Placeholder");
	intro_ui_text->SetPos(Vector2(config["INTRO_OVERLAY"]["text_pos"][0], config["INTRO_OVERLAY"]["text_pos"][1])); 

	//Load in UI: countdown frames (and set to default)
	for (int i = 0; i < 4; i++) {
		ImageGO2D* countdown_sprite = new ImageGO2D(config["COUNTDOWN_FRAMES"]["sprites"][i]);
		countdown_sprite->SetPos(Vector2(
			config["COUNTDOWN_FRAMES"]["position"][0] + (Locator::getRD()->m_window_width / 2.f), 
			config["COUNTDOWN_FRAMES"]["position"][1] + (Locator::getRD()->m_window_height / 2.f)), false);
		countdown_sprite->CentreOrigin();
		countdown_ui_sprites.push_back(countdown_sprite);
	}

	//Load in UI: lap count
	for (int i = 0; i < 3; i++) {
		ImageGO2D* lap_sprite = new ImageGO2D(config["LAP_" + std::to_string(i + 1)]["sprite"]);
		lap_sprite->SetPos(Vector2(config["LAP_" + std::to_string(i + 1)]["position"][0], config["LAP_" + std::to_string(i + 1)]["position"][1]));
		lap_ui_sprites.push_back(lap_sprite);
	}

	//Load in UI: position count
	for (int i = 0; i < 12; i++) {
		ImageGO2D* pos_sprite = new ImageGO2D(config["POS_" + std::to_string(i + 1)]["sprite"]);
		pos_sprite->SetPos(Vector2(config["POS_" + std::to_string(i + 1)]["position"][0], config["POS_" + std::to_string(i + 1)]["position"][1]));
		position_ui_sprites.push_back(pos_sprite);
	}

	//Load in UI: item UI
	item_ui_sprite = new ImageGO2D(config["ITEM_HOLDER"]["sprite"]);
	item_ui_sprite->SetPos(Vector2(config["ITEM_HOLDER"]["position"][0], config["ITEM_HOLDER"]["position"][1]));
	HideItemSpinner();

	//Load in UI: course outro (finished)
	outro_ui_sprite = new ImageGO2D(config["OUTRO_OVERLAY"]["sprite"]);
	outro_ui_sprite->SetPos(Vector2(config["OUTRO_OVERLAY"]["position"][0], config["OUTRO_OVERLAY"]["position"][1]));
}

/* Unload all UI sprites */
void InGameUI::ExpensiveUnload()
{
	if (intro_ui_sprite != nullptr) {
		intro_ui_sprite->Reset();
		intro_ui_text->Reset();
	}
	lap_ui_sprites.clear();
	position_ui_sprites.clear();
	if (item_ui_sprite != nullptr) {
		item_ui_sprite->Reset();
	}
	outro_ui_sprite->Reset();

	current_state = InGameInterfaceState::UI_HIDDEN;
	ui_offset = Vector2(0, 0);
	ui_size = Vector2(0, 0);
	countdown_size_log = 0.0f;
}

/* Set the current UI state */
void InGameUI::SetState(InGameInterfaceState state)
{
	if (current_state == state) {
		return;
	}
	DebugText::print("InGameUI: Set state to " + std::to_string((int)state) + ".");

	//Set state
	current_state = state;

	//If race is over, reposition our position sprite (it's now locked)
	if (current_state == InGameInterfaceState::UI_RACE_OVER) {
		position_ui_sprite->SetPos(Vector2(config["OUTRO_OVERLAY"]["text_pos"][0], config["OUTRO_OVERLAY"]["text_pos"][1]));
	}
}

/* Set the name of the current map */
void InGameUI::SetMapName(const std::string & name)
{
	intro_ui_text->SetText(name);
}

/* Progress to the next countdown frame */
void InGameUI::SetCountdownFrame(int frame)
{
	if (frame == countdown_ui_sprites.size() || countdown_ui_sprite == countdown_ui_sprites.at(frame)) {
		return;
	}
	DebugText::print("InGameUI: Set countdown frame to " + std::to_string(frame) + ".");

	countdown_ui_sprite = countdown_ui_sprites.at(frame);
	countdown_size_log = 0.0f;
}

/* Update current UI item */
void InGameUI::SetCurrentItem(ItemType item)
{
	item_image_sprite = Locator::getItemData()->GetItemSprite(item, ui_id);
	item_image_sprite->SetPos(Vector2(config["ITEM_HOLDER"]["text_pos"][0], config["ITEM_HOLDER"]["text_pos"][1]));
	item_image_sprite->SetScale(config["ITEM_HOLDER"]["item_scale"]);
}

/* Update current lap (e.g. 1/3, 2/3, 3/3) */
void InGameUI::SetCurrentLap(int lap)
{
	if (lap > lap_ui_sprites.size()) {
		DebugText::print("InGameUI: Tried to set lap to " + std::to_string(lap) + ", which is out of the API's scope!");
		return;
	}

	lap_ui_sprite = lap_ui_sprites.at(lap - 1);
}

/* Update the player position (e.g. 1st, 2nd, 3rd) */
void InGameUI::SetPlayerPosition(int position)
{
	if (position > position_ui_sprites.size()) {
		DebugText::print("InGameUI: Tried to set player's position to " + std::to_string(position) + ", which is out of the API's scope!");
		return;
	}
	if (current_state == InGameInterfaceState::UI_RACE_OVER) {
		DebugText::print("InGameUI: Player has finished race, ignoring call to set position!");
		return;
	}

	position_ui_sprite = position_ui_sprites.at(position - 1);
}

/* Starts the automated item spinner to land on a pre-determined item */
void InGameUI::ShowItemSpinner(Player* this_player)
{
	if (item_ui_sprite == nullptr) {
		DebugText::print("InGameUI: Call to show item spinner before the sprite was loaded. Are you calling ExpensiveLoad?");
		return;
	}

	//Show spinner
	item_ui_sprite->SetVisible(true);

	//The item we'll end on
	ItemType item_to_give = Locator::getItemData()->GetRandomItem(this_player->GetRanking());

	//Give the item once the animation is done
	if (this_player->SetItemInInventory(item_to_give)) {
		SetCurrentItem(item_to_give);
	}
}

/* Hides the item spinner (item used) */
void InGameUI::HideItemSpinner()
{
	if (item_ui_sprite == nullptr) {
		DebugText::print("InGameUI: Call to hide item spinner before the sprite was loaded. Are you calling ExpensiveLoad?");
		return;
	}
	DebugText::print("InGameUI: Hiding item spinner.");

	item_ui_sprite->SetVisible(false);
	SetCurrentItem(PLACEHOLDER);
}

/* Render the current UI - should ONLY ever be called in a scene's Render2D! */
void InGameUI::Render()
{
	switch (current_state) {
		case InGameInterfaceState::UI_HIDDEN: {
			return;
		}
		case InGameInterfaceState::UI_COURSE_INTRO: {
			if (intro_ui_sprite != nullptr) { 
				intro_ui_sprite->Render();
				intro_ui_text->Render();
			}
			break;
		}
		case InGameInterfaceState::UI_COUNTDOWN: {
			if (countdown_ui_sprite != nullptr) { countdown_ui_sprite->Render(); }
		}
		case InGameInterfaceState::UI_RACING: {
			if (lap_ui_sprite != nullptr) { lap_ui_sprite->Render(); }
			if (position_ui_sprite != nullptr) { position_ui_sprite->Render(); }
			if (item_image_sprite != nullptr) { item_image_sprite->Render(); }
			if (item_ui_sprite != nullptr) { item_ui_sprite->Render(); }

			//Render current timers
			//Coming soon(TM)

			break;
		}
		case InGameInterfaceState::UI_RACE_OVER: {
			if (outro_ui_sprite != nullptr) { outro_ui_sprite->Render(); }
			if (position_ui_sprite != nullptr) { position_ui_sprite->Render(); }
			break;
		}
	}
}

/* Update the current UI */
void InGameUI::Update()
{
	switch (current_state) {
		case InGameInterfaceState::UI_HIDDEN: {
			return;
		}
		case InGameInterfaceState::UI_COURSE_INTRO: {
			break;
		}
		case InGameInterfaceState::UI_COUNTDOWN: {
			if (countdown_ui_sprite != nullptr) { 
				countdown_ui_sprite->SetScale(Vector2(1 + (countdown_size_log / 10), 1 + (countdown_size_log / 10)));
				countdown_size_log += (float)Locator::getGSD()->m_dt;
			}
			break;
		}
		case InGameInterfaceState::UI_RACING: {
			//We'll wanna update lap/race counters here soon(TM)
			break;
		}
		case InGameInterfaceState::UI_RACE_OVER: {
			break;
		}
	}
}
