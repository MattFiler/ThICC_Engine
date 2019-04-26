#pragma once
#include "Item.h"
#include "MoveAI.h"
#include "ControlledMovement.h"
class RedShell : public Item
{
public:
	RedShell();
	~RedShell() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* _player) override;
	void Use(Player* _player, bool _altUse) override;

private:
	std::unique_ptr<MoveAI> m_ai = nullptr;
	std::unique_ptr<ControlledMovement> m_move = nullptr;
	
	float m_altUseSpeed = 60;
};

