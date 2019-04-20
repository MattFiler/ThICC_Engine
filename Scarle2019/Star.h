#pragma once
#include "Item.h"
class Star :public Item
{
public:
	Star();
	~Star() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	float m_boostAmount = 40;
};

