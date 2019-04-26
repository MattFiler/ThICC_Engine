#pragma once

#include "Item.h"
#include "ItemCollisionData.h"
#include "ItemThrowData.h"

class Player;

class Banana : public Item {
public:
	Banana();
	void initItemData();
	~Banana() = default;

	virtual void Tick() override { Item::Tick(); };

	void Use(Player* player, bool _altUse) override;
	void HitByPlayer(Player* _player) override;

private:
	ItemThrowData m_throwData;
	ItemCollisionData m_collisionData;
};