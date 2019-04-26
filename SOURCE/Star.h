#pragma once

#include "Item.h"

class Player;

class Star :public Item
{
public:
	Star();
	void InitStarData();
	~Star() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	float m_boostAmount = 40;
	float m_playerDrag = 0.7;
};

