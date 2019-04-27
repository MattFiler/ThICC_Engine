#pragma once

#include "pch.h"
#include "Item.h"
#include "ItemThrowData.h"
#include "ItemCollisionData.h"

class Player;

class FakeItemBox : public Item
{
public:
	FakeItemBox();
	void InitBoxData();
	~FakeItemBox() = default;

	void Tick() { Item::Tick(); };

	void Use(Player* player, bool _altUse);
	void HitByPlayer(Player* player);

private:
	ItemThrowData m_throwData;
	ItemCollisionData m_collisionData;
};