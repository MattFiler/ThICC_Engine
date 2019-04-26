#pragma once

#include "pch.h"
#include "Player.h"
#include "ItemCollisionData.h"
#include "ItemGrowthData.h"
#include <json.hpp>
using json = nlohmann::json;

class Explosion : public TrackMagnet
{
public:
	Explosion(ItemType _ownerType);
	void InitExplosionData(ItemType _ownerType);
	~Explosion() = default;

	void HitByPlayer(Player* _player);
	virtual void Tick() override;

	void explode() { m_explode = true; };
private:
	bool m_explode = false;
	
	ItemGrowthData m_growthData;
	ItemCollisionData m_collisionData;
};

