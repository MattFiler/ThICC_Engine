#pragma once

#include "Item.h"
#include "Explosion.h"
class Bomb : public Item
{
public:
	Bomb(std::function<Explosion*()> _CreateExplosionFunciton);
	~Bomb() = default;

	virtual void Tick() override;

	void Use(Player* player, bool _altUse) override;
	void HitByPlayer(Player* player) override;

private:
	std::function<Explosion*()> CreateExplosion;
	Explosion* explosion;
};