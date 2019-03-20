#pragma once

#include "Item.h"

class Mushroom : public Item {
public:
	Mushroom();
	~Mushroom() = default;

	virtual void Tick() override { Item::Tick(); };
	void HitByPlayer(Player* player) override;
	void Use(Player* player) override;
};