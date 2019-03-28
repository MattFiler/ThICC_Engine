#pragma once

#include "Item.h"

class Bomb : public Item
{
public:
	Bomb();
	~Bomb() = default;

	virtual void Tick() override;

	void Use(Player* player, bool _altUse) override;
	void HitByPlayer(Player* player) override;

private:

};