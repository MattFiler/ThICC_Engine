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
	float m_durationMulti = 12;
	float m_playerMoveSpeed = 0;
	float m_playerTurnSpeed = 0;
};

