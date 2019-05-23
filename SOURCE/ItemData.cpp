#include "pch.h"
#include "ItemData.h"
#include <fstream>
#include <random>
#include <iterator>
#include <map>

/* Load config data for each item */
ItemData::ItemData() {
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	complete_config << i;

	ProcessConfig(GREEN_SHELL, "GREEN_SHELL");
	ProcessConfig(RED_SHELL, "RED_SHELL");
	ProcessConfig(BANANA, "BANANA");
	ProcessConfig(FAKE_BOX, "FAKE_BOX");
	ProcessConfig(MUSHROOM, "MUSHROOM");
	ProcessConfig(MUSHROOM_3X, "MUSHROOM_3X");
	ProcessConfig(BOMB, "BOMB");
	ProcessConfig(BLUE_SHELL, "BLUE_SHELL");
	ProcessConfig(LIGHTNING_BOLT, "LIGHTNING_BOLT");
	ProcessConfig(STAR, "STAR");
	ProcessConfig(MUSHROOM_UNLIMITED, "MUSHROOM_UNLIMITED");
	ProcessConfig(MUSHROOM_GIANT, "MUSHROOM_GIANT");
	ProcessConfig(SQUID, "SQUID");
	ProcessConfig(POW, "POW");
	ProcessConfig(LIGHTNING_CLOUD, "LIGHTNING_CLOUD");
	ProcessConfig(BULLET_BILL, "BULLET_BILL");
	ProcessConfig(GREEN_SHELL_3X, "GREEN_SHELL_3X");
	ProcessConfig(RED_SHELL_3X, "RED_SHELL_3X");
	ProcessConfig(BANANA_3X, "BANANA_3X");

	//Set item placeholder sprite
	for (int i = 0; i < 4; i++) {
		item_sprite_image[19][i] = new ImageGO2D("ITEM_PLACEHOLDER");
	}
}

/* Get a random item based on the player position */
ItemType ItemData::GetRandomItem(int _position) {

	return POW;
	float random_out_of_100 = (rand() % 101);

	for (int i = 0; i < item_count-1; i++) {
		ItemType this_item = static_cast<ItemType>(i);
		if (random_out_of_100 <= GetCumulativeProbability(this_item, _position)) {
			return this_item;
		}
	}

	return BANANA_3X;
}

/* Get the solo probability of getting an item from the mystery box at a position in the race (e.g. 1st) */
float ItemData::GetProbability(ItemType _item, int _position) {
	IdiotCheck(_item);
	return item_probability[static_cast<int>(_item)][_position - 1];
}

/* Work out item probability out of 100 cumulatively */
float ItemData::GetCumulativeProbability(ItemType _item, int _position) {
	IdiotCheck(_item);

	int item_id = static_cast<int>(_item);
	float return_probability = 0.0f;
	for (int i = item_id; i >= 0; i--) {
		return_probability += item_probability[item_id - i][_position - 1];
	}

	return return_probability;
}

/* Return the item's sprite name for the UI */
std::string ItemData::GetItemSpriteName(ItemType _item) {
	IdiotCheck(_item);
	return item_sprite[static_cast<int>(_item)];
}

/* Return the name of the model for an item */
std::string ItemData::GetItemModelName(ItemType _item) {
	IdiotCheck(_item);
	return item_model[static_cast<int>(_item)];
}

/* Return the actual sprite for an item */
ImageGO2D* ItemData::GetItemSprite(ItemType _item, int _player_id) {
	return item_sprite_image[static_cast<int>(_item)][_player_id];
}

/* Process the config for an item type, and set the probability array */
void ItemData::ProcessConfig(ItemType _item, const std::string& config_name) {
	IdiotCheck(_item);

	//Adjust config for item/default params
	json temp_config_specialised = complete_config[config_name];
	json temp_config_default = complete_config["DEFAULT"];
	for (json::iterator i = temp_config_specialised.begin(); i != temp_config_specialised.end(); ++i)
	{
		if (!temp_config_specialised[i.key()].is_null() && !temp_config_specialised[i.key()].is_object())
		{
			temp_config_default[i.key()] = i.value();
		}
		else if (temp_config_specialised[i.key()].is_object())
		{
			for (json::iterator x = temp_config_specialised[i.key()].begin(); x != temp_config_specialised[i.key()].end(); ++x)
			{
				if (!temp_config_specialised[i.key()][x.key()].is_null())
				{
					temp_config_default[i.key()][x.key()] = x.value();
				}
			}
		}
	}

	//Update item probabilities
	int item_id = static_cast<int>(_item);
	for (int i = 0; i < 12; i++) {
		item_probability[item_id][i] = temp_config_default["probabilities"]["place_" + std::to_string(i + 1)];
		item_sprite[item_id] = temp_config_default["ui_sprite"];
		item_model[item_id] = temp_config_default["model"];
	}

	//Set item sprites
	for (int i = 0; i < 4; i++) {
		item_sprite_image[item_id][i] = new ImageGO2D(item_sprite[item_id]);
	}
}
