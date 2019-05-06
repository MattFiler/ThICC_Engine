#pragma once

#include "pch.h"
#include "Player.h"
#include "ItemCollisionData.h"
#include "ItemGrowthData.h"
#include "AnimationController.h"
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

	void Explode() { m_explode = true; };
	void Render() { m_displayedMesh->Render(); }

	AnimationController* GetDisplayedMesh() { return m_displayedMesh; };
private:
	bool m_explode = false;
	
	ItemGrowthData m_growthData;
	ItemCollisionData m_collisionData;

	AnimationController* m_displayedMesh;
};

