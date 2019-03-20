#pragma once

#include "GameFilepaths.h"
#include "TrackMagnet.h"
#include "ServiceLocator.h"
#include "ItemData.h"
#include <json.hpp>
using json = nlohmann::json;

class Player;

class Item : public TrackMagnet {
public:
	Item(const std::string& item_type);
	~Item() = default;

	virtual void Tick() override { TrackMagnet::Tick(); };
	virtual void HitByPlayer(Player* player) {};
protected:
	float m_probability_modifier[12] = {};
	json m_item_config;
	GameFilepaths m_filepath;
	std::string m_model_name = "";
};