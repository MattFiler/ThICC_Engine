#pragma once

#include "Constants.h"
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
	std::string GetItemSpritePath(ItemType _item);
	std::string GetItemModelName(ItemType _item);

private:
	void ProcessConfig(ItemType _item, const std::string& config_name);

	json complete_config;

	const int item_count = 19; //must match arrays below
	float item_probability[19][12];
	std::string item_sprite[19];
	std::string item_model[19];
};