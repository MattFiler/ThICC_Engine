#pragma once

#include "Item.h"
#include "ItemBoostData.h"

class Player;

class GoldenMushroom : public Item
{
public:
	GoldenMushroom();
	void InitItemData();
	~GoldenMushroom() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	ItemBoostData m_boostData;
};

