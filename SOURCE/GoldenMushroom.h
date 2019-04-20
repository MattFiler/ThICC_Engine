#pragma once

#include "Item.h"

class Player;

class GoldenMushroom : public Item
{
public:
	GoldenMushroom();
	~GoldenMushroom() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	float m_boostTimeElapsed = 0;
	float m_boostAmount = 35;
	float m_minVelo = 50;
	double m_boostDuration = 1;
};

