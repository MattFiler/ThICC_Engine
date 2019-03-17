#pragma once

#include "GameFilepaths.h"
#include <json.hpp>
using json = nlohmann::json;

class Item : public TrackMagnet {
public:
	Item(const std::string& item_type);
	~Item() = default;

	float getProbability(int _position) { return m_probability_modifier[_position]; };

protected:
	float m_probability_modifier[12] = {};
	json m_item_config;
	GameFilepaths m_filepath;
	std::string m_model_name = "";
};