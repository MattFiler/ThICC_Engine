#include "pch.h"
#include "Item.h"
#include <fstream>

Item::Item(const std::string& item_type) : TrackMagnet(item_type) {
	//Read in item config
	std::ifstream i("DATA/ITEMS/ITEM_CONFIG.JSON");
	m_item_config << i;

	//Adjust config for item/default params
	json temp_config = m_item_config[item_type];
	m_item_config = m_item_config["DEFAULT"];
	for (json::iterator i = temp_config.begin(); i != temp_config.end(); ++i)
	{
		if (!temp_config[i.key()].is_null() && !temp_config[i.key()].is_object())
		{
			m_item_config[i.key()] = i.value();
		}
		else if (temp_config[i.key()].is_object())
		{
			for (json::iterator x = temp_config[i.key()].begin(); x != temp_config[i.key()].end(); ++x)
			{
				if (!temp_config[i.key()][x.key()].is_null())
				{
					m_item_config[i.key()][x.key()] = x.value();
				}
			}
		}
	}

	//Update item probabilities
	for (int i = 0; i < 12; i++) {
		m_probability_modifier[i] = m_item_config["probabilities"]["place_" + std::to_string(i+1)];
	}

	//Set model name
	std::string model_name = m_item_config["model"];
	m_model_name = model_name;

	SetDrag(0.7);
	SetPhysicsOn(true);
}