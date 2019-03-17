#pragma once

#include "Item.h"

class Banana : public Item {
public:
	Banana();
	~Banana() = default;

	void HitByPlayer(Player* player) override;
};