#pragma once

#include "Item.h"
#include "ItemThrowData.h"

class Explosion;
class Player;

class Bomb : public Item
{
public:
	Bomb(std::function<Explosion*(ItemType)> _CreateExplosionFunciton);
	void InitBombData();
	~Bomb() = default;

	virtual void Tick() override;

	void Use(Player* player, bool _altUse) override;
	void Detonate();
	void HitByPlayer(Player* player) override;

	Explosion* GetExplosion() { return m_explosion; };
private:
	std::function<Explosion*(ItemType)> CreateExplosion;
	Explosion* m_explosion;
	float m_current_time = 0;
	float m_countdownTime = 0;
	bool m_countdown = false;

	ItemThrowData m_throwData;
};