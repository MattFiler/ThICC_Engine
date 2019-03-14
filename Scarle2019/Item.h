#pragma once

#include "GameFilepaths.h"
#include <json.hpp>
using json = nlohmann::json;

class Item {
public:
	Item(const std::string& item_type);
	~Item() = default;

protected:
	float m_probability_modifier[12] = {};
	json m_item_config;
	GameFilepaths m_filepath;
};