#pragma once

#include "pch.h"
#include "Item.h"

class Player;

class FakeItemBox : public Item
{
public:
	FakeItemBox();
	~FakeItemBox() = default;

	void Tick() { Item::Tick(); };

	void Use(Player* player, bool _altUse);
	void HitByPlayer(Player* player);
};