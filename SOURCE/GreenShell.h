#pragma once

#include "Item.h"

class Player;

class GreenShell : public Item
{
public:
	GreenShell();
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

	//Player collision data
	Vector3 m_playerVel = Vector3::Zero;
	float m_playerJumpHeight = 0;
	float m_playerJumpDuration = 0;
	float m_playerFlipRev = 0;
	float m_playerFlipDuration = 0;
	float m_playerVerticalPosOffset = 0;
};