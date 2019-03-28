#pragma once

#include "Item.h"

class GreenShell : public Item 
{
public:
	GreenShell();
	~GreenShell() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* _player) override;
	void Use(Player* _player, bool _altUse) override;

private:
	Player * m_player = nullptr;

	double m_elapsedTime = 0;
	double m_maxDuration = 20;
};