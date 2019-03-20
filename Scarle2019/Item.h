#pragma once

#include "GameFilepaths.h"
#include "TrackMagnet.h"
#include "ServiceLocator.h"
#include "ItemData.h"

class Player;

class Item : public TrackMagnet {
public:
	Item(const std::string& item_type);
	~Item() = default;

	virtual void Tick() override { TrackMagnet::Tick(); };
	virtual void HitByPlayer(Player* player) {};
};