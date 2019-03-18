#include "pch.h"
#include "ItemProbability.h"
#include <fstream>
#include <random>
#include <iterator>
#include <map>

/* Set the probability percentages for items */
ItemProbability::ItemProbability() {
	std::ifstream i("DATA/ITEMS/ITEM_CONFIG.JSON");
	complete_config << i;

	ProcessConfig(green_shell_probability, "GREEN_SHELL");
	ProcessConfig(red_shell_probability, "RED_SHELL");
	ProcessConfig(banana_probability, "BANANA");
	ProcessConfig(fake_box_probability, "FAKE_BOX");
	ProcessConfig(mushroom_probability, "MUSHROOM");
	ProcessConfig(mushroom_3x_probability, "MUSHROOM_3X");
	ProcessConfig(bomb_probability, "BOMB");
	ProcessConfig(blue_shell_probability, "BLUE_SHELL");
	ProcessConfig(lightning_bolt_probability, "LIGHTNING_BOLT");
	ProcessConfig(star_probability, "STAR");
	ProcessConfig(mushroom_unlimited_probability, "MUSHROOM_UNLIMITED");
	ProcessConfig(mushroom_giant_probability, "MUSHROOM_GIANT");
	ProcessConfig(squid_probability, "SQUID");
	ProcessConfig(pow_probability, "POW");
	ProcessConfig(lightning_cloud_probability, "LIGHTNING_CLOUD");
	ProcessConfig(bullet_bill_probability, "BULLET_BILL");
	ProcessConfig(green_shell_3x_probability, "GREEN_SHELL_3X");
	ProcessConfig(red_shell_3x_probability, "RED_SHELL_3X");
	ProcessConfig(banana_3x_probability, "BANANA_3X");
}

/* Get a random item based on the player position */
ItemType ItemProbability::GetRandomItem(int _position) {
	float random_out_of_100 = (rand() % 101);

	if (random_out_of_100 <= GetCumulativeProbability(GREEN_SHELL, _position)) {
		return GREEN_SHELL;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(RED_SHELL, _position)) {
		return RED_SHELL;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(BANANA, _position)) {
		return BANANA;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(FAKE_BOX, _position)) {
		return FAKE_BOX;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(MUSHROOM, _position)) {
		return MUSHROOM;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(MUSHROOM_3X, _position)) {
		return MUSHROOM_3X;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(BOMB, _position)) {
		return BOMB;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(BLUE_SHELL, _position)) {
		return BLUE_SHELL;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(LIGHTNING_BOLT, _position)) {
		return LIGHTNING_BOLT;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(STAR, _position)) {
		return STAR;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(RED_SHELL, _position)) {
		return RED_SHELL;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(MUSHROOM_UNLIMITED, _position)) {
		return MUSHROOM_UNLIMITED;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(MUSHROOM_GIANT, _position)) {
		return MUSHROOM_GIANT;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(SQUID, _position)) {
		return SQUID;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(POW, _position)) {
		return POW;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(LIGHTNING_CLOUD, _position)) {
		return LIGHTNING_CLOUD;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(BULLET_BILL, _position)) {
		return BULLET_BILL;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(GREEN_SHELL_3X, _position)) {
		return GREEN_SHELL_3X;
	}
	else if (random_out_of_100 <= GetCumulativeProbability(RED_SHELL_3X, _position)) {
		return RED_SHELL_3X;
	}
	return BANANA_3X;
}

/* Get the solo probability of getting an item from the mystery box at a position in the race (e.g. 1st) */
float ItemProbability::GetProbability(ItemType _item, int _position) {
	switch (_item) {
		case GREEN_SHELL: {
			return green_shell_probability[_position];
		}
		case RED_SHELL: {
			return red_shell_probability[_position];
		}
		case BANANA: {
			return banana_probability[_position];
		}
		case FAKE_BOX: {
			return fake_box_probability[_position];
		}
		case MUSHROOM: {
			return mushroom_probability[_position];
		}
		case MUSHROOM_3X: {
			return mushroom_3x_probability[_position];
		}
		case BOMB: {
			return bomb_probability[_position];
		}
		case BLUE_SHELL: {
			return blue_shell_probability[_position];
		}
		case LIGHTNING_BOLT: {
			return lightning_bolt_probability[_position];
		}
		case STAR: {
			return star_probability[_position];
		}
		case MUSHROOM_UNLIMITED: {
			return mushroom_unlimited_probability[_position];
		}
		case MUSHROOM_GIANT: {
			return mushroom_giant_probability[_position];
		}
		case SQUID: {
			return squid_probability[_position];
		}
		case POW: {
			return pow_probability[_position];
		}
		case LIGHTNING_CLOUD: {
			return lightning_cloud_probability[_position];
		}
		case BULLET_BILL: {
			return bullet_bill_probability[_position];
		}
		case GREEN_SHELL_3X: {
			return green_shell_3x_probability[_position];
		}
		case RED_SHELL_3X: {
			return red_shell_3x_probability[_position];
		}
		case BANANA_3X: {
			return banana_3x_probability[_position];
		}
	}
	return 0.0f;
}

/* Work out item probability out of 100 cumulatively */
float ItemProbability::GetCumulativeProbability(ItemType _item, int _position) {
	switch (_item) {
		case GREEN_SHELL: {
			return green_shell_probability[_position];
		}
		case RED_SHELL: {
			return red_shell_probability[_position] + green_shell_probability[_position];
		}
		case BANANA: {
			return banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case FAKE_BOX: {
			return fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case MUSHROOM: {
			return mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case MUSHROOM_3X: {
			return mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + 
				green_shell_probability[_position];
		}
		case BOMB: {
			return bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] + 
				red_shell_probability[_position] + green_shell_probability[_position];
		}
		case BLUE_SHELL: {
			return blue_shell_probability[_position] + bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] +
				banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case LIGHTNING_BOLT: {
			return lightning_bolt_probability[_position] + blue_shell_probability[_position] + bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] + 
				fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case STAR: {
			return star_probability[_position] + lightning_bolt_probability[_position] + blue_shell_probability[_position] + bomb_probability[_position] + mushroom_3x_probability[_position] +
				mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case MUSHROOM_UNLIMITED: {
			return mushroom_unlimited_probability[_position] + star_probability[_position] + lightning_bolt_probability[_position] + blue_shell_probability[_position] + bomb_probability[_position] +
				mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] +
				green_shell_probability[_position];
		}
		case MUSHROOM_GIANT: {
			return mushroom_giant_probability[_position] + mushroom_unlimited_probability[_position] + star_probability[_position] + lightning_bolt_probability[_position] + blue_shell_probability[_position] +
				bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] + 
				red_shell_probability[_position] + green_shell_probability[_position];
		}
		case SQUID: {
			return squid_probability[_position] + mushroom_giant_probability[_position] + mushroom_unlimited_probability[_position] + star_probability[_position] + lightning_bolt_probability[_position] +
				blue_shell_probability[_position] + bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] +
				banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case POW: {
			return pow_probability[_position] + squid_probability[_position] + mushroom_giant_probability[_position] + mushroom_unlimited_probability[_position] + star_probability[_position] +
				lightning_bolt_probability[_position] + blue_shell_probability[_position] + bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] +
				fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case LIGHTNING_CLOUD: {
			return lightning_cloud_probability[_position] + pow_probability[_position] + squid_probability[_position] + mushroom_giant_probability[_position] + mushroom_unlimited_probability[_position] + 
				star_probability[_position] + lightning_bolt_probability[_position] + blue_shell_probability[_position] + bomb_probability[_position] + mushroom_3x_probability[_position] + 
				mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case BULLET_BILL: {
			return bullet_bill_probability[_position] + lightning_cloud_probability[_position] + pow_probability[_position] + squid_probability[_position] + mushroom_giant_probability[_position] +
				mushroom_unlimited_probability[_position] + star_probability[_position] + lightning_bolt_probability[_position] + blue_shell_probability[_position] + bomb_probability[_position] +
				mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case GREEN_SHELL_3X: {
			return green_shell_3x_probability[_position] + bullet_bill_probability[_position] + lightning_cloud_probability[_position] + pow_probability[_position] + squid_probability[_position] + 
				mushroom_giant_probability[_position] + mushroom_unlimited_probability[_position] + star_probability[_position] + lightning_bolt_probability[_position] + blue_shell_probability[_position] + 
				bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] + banana_probability[_position] +
				red_shell_probability[_position] + green_shell_probability[_position];
		}
		case RED_SHELL_3X: {
			return red_shell_3x_probability[_position] + green_shell_3x_probability[_position] + bullet_bill_probability[_position] + lightning_cloud_probability[_position] + pow_probability[_position] + 
				squid_probability[_position] + mushroom_giant_probability[_position] + mushroom_unlimited_probability[_position] + star_probability[_position] + lightning_bolt_probability[_position] +
				blue_shell_probability[_position] + bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] + fake_box_probability[_position] +
				banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
		case BANANA_3X: {
			return banana_3x_probability[_position] + red_shell_3x_probability[_position] + green_shell_3x_probability[_position] + bullet_bill_probability[_position] + lightning_cloud_probability[_position] + 
				pow_probability[_position] + squid_probability[_position] + mushroom_giant_probability[_position] + mushroom_unlimited_probability[_position] + star_probability[_position] +
				lightning_bolt_probability[_position] + blue_shell_probability[_position] + bomb_probability[_position] + mushroom_3x_probability[_position] + mushroom_probability[_position] +
				fake_box_probability[_position] + banana_probability[_position] + red_shell_probability[_position] + green_shell_probability[_position];
		}
	}
	return 0.0f;
}

/* Process the config for an item type, and set the probability array */
void ItemProbability::ProcessConfig(float* config_to_set, const std::string& config_name) {
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
	for (int i = 0; i < 12; i++) {
		config_to_set[i] = temp_config_default["probabilities"]["place_" + std::to_string(i + 1)];
	}
}
