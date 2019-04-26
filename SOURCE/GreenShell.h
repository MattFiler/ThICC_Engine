#pragma once

#include "Item.h"
#include "ItemCollisionData.h"

class Player;

class GreenShell : public Item
{
public:
	GreenShell();
	void InitItemData();
	~GreenShell() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* _player) override;
	void Use(Player* _player, bool _altUse) override;

private:
	//Use data
	float m_speed = 0;
	float m_usePosOffset = 0;

	//Spin data
	float m_spinRev = 0;
	float m_spinDuration = 0;

	ItemCollisionData m_collisionData;
};