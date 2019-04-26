#pragma once

#include "pch.h"
#include "Player.h"
#include "ItemCollisionData.h"
#include <json.hpp>
using json = nlohmann::json;

class Explosion : public TrackMagnet
{
public:
	Explosion(ItemType _ownerType);
	void initExplosionData(ItemType _ownerType);
	~Explosion() = default;

	void HitByPlayer(Player* _player);
	virtual void Tick() override;

	void explode() { m_explode = true; };
private:
	bool m_explode = false;
	Vector3 m_startSize;
	Vector3 m_endSize;
	float m_sizeMulti = 0;
	float m_percent = 0;
	float m_speed = 1.5;

	ItemCollisionData m_collisionData;
};

