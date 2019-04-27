#pragma once
#include "Item.h"
#include "MoveAI.h"
#include "ControlledMovement.h"
#include "ItemCollisionData.h"
class RedShell : public Item
{
public:
	RedShell();
	void InitShellData();
	~RedShell() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* _player) override;
	void Use(Player* _player, bool _altUse) override;

private:
	std::unique_ptr<MoveAI> m_ai = nullptr;
	std::unique_ptr<ControlledMovement> m_move = nullptr;
	
	float m_usePosOffset = 0;
	float m_maxSpeed = 0;
	float m_moveSpeed = 0;
	float m_turnSpeed = 0;
	float m_moveTowardDistSqrd = 0;
	bool m_altUse = false;

	ItemCollisionData m_collisionData;
};

