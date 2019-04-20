#pragma once

#include "Item.h"

class Mushroom : public Item {
public:
	Mushroom();
	~Mushroom() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	float m_boostTimeElapsed = 0;
	float m_boostAmount = 40;
	float m_minVelo = 50;
	double m_boostDuration = 2;
};