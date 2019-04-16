#pragma once

#include "pch.h"
#include "Item.h"

class Explosion;

class Bomb : public Item
{
public:
	Bomb(std::function<Explosion*()> _CreateExplosionFunciton);
	~Bomb() = default;

	virtual void Tick() override;

	void Use(Player* player, bool _altUse) override;
	void Detonate();
	void HitByPlayer(Player* player) override;

private:
	std::function<Explosion*()> CreateExplosion;
	Explosion* explosion;
	float m_current_time = 0;
	float m_countdown_time = 4;
	bool m_countdown = false;
};