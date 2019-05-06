#pragma once
#include "Item.h"
#include "ItemAiData.h"
#include <json.hpp>
using json = nlohmann::json;

class BulletBill : public Item
{
public:
	BulletBill();
	void InitBulletData();
	~BulletBill() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	ItemAiData m_aiData;
	float m_durationMulti = 0;
	float m_bulletDuration = 0;
	float m_bulletTimeElapsed = 0;
};

