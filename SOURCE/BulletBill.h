#pragma once
#include "Item.h"
#include "ItemAiData.h"

class BulletBill : public Item
{
public:
	BulletBill();
	~BulletBill() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	ItemAiData m_aiData;
	float m_durationMulti = 1;
	float m_playerMoveSpeed = 0;
	float m_playerTurnSpeed = 0;
	float m_bulletDuration = 2;
	float m_bulletTimeElapsed = 0;
};

