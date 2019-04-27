#pragma once

#include "Item.h"
#include "ItemGrowthData.h"

class Player;

class GiantMushroom :public Item
{
public:
	GiantMushroom();
	void InitMushroomData();
	~GiantMushroom() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	ItemGrowthData m_growthData;
};

