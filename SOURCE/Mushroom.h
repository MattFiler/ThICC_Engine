#pragma once

#include "Item.h"
#include "ItemBoostData.h"
#include <json.hpp>
using json = nlohmann::json;

class Player;

class Mushroom : public Item {
public:
	Mushroom();
	void InitItemData();
	~Mushroom() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	ItemBoostData m_boostData;
};