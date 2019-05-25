#pragma once

#include "Constants.h"
#include "GameFilepaths.h"
#include "ImageGO2D.h"
#include <json.hpp>
using json = nlohmann::json;

class ItemData {
public:
	ItemData();
	~ItemData() = default;

	/* Return a random item based on player position */
	ItemType GetRandomItem(int _position);

	/* Return probability data */
	float GetProbability(ItemType _item, int _position);
	float GetCumulativeProbability(ItemType _item, int _position);

	/* Return item sprite/model info */
	std::string GetItemSpriteName(ItemType _item);
	std::string GetItemModelName(ItemType _item);

	/* Return item actual sprite reference */
	ImageGO2D* GetItemSprite(ItemType _item, int _player_id);

	bool CanDrop(ItemType _item);

private:
	void ProcessConfig(ItemType _item, const std::string& config_name);

	void IdiotCheck(ItemType _item) {
		if (_item == PLACEHOLDER) {
			throw std::runtime_error("CANNOT CALL PLACEHOLDER ITEM.");
		}
	};

	json complete_config;

	const int item_count = 19; //must match arrays below
	float item_probability[19][12];
	std::string item_sprite[19];
	std::string item_model[19];
	ImageGO2D* item_sprite_image[20][4]; // LAST IN ARRAY IS PLACEHOLDER!

	GameFilepaths m_filepath;
};