#pragma once

#include "Constants.h"
#include <json.hpp>
using json = nlohmann::json;

class ItemProbability {
public:
	ItemProbability();
	~ItemProbability() = default;

	ItemType GetRandomItem(int _position);
	float GetProbability(ItemType _item, int _position);

private:
	void ProcessConfig(float* config_to_set, const std::string& config_name);

	json complete_config;

	float green_shell_probability[12];
	float red_shell_probability[12];
	float banana_probability[12];
	float fake_box_probability[12];
	float mushroom_probability[12];
	float mushroom_3x_probability[12];
	float bomb_probability[12];
	float blue_shell_probability[12];
	float lightning_bolt_probability[12];
	float star_probability[12];
	float mushroom_unlimited_probability[12];
	float mushroom_giant_probability[12];
	float squid_probability[12];
	float pow_probability[12];
	float lightning_cloud_probability[12];
	float bullet_bill_probability[12];
	float green_shell_3x_probability[12];
	float red_shell_3x_probability[12];
	float banana_3x_probability[12];
};