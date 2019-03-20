#pragma once

#include "Item.h"

class GreenShell : public Item 
{
public:
	GreenShell();
	~GreenShell() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override;
	void Use(Player* player) override;

private:
	Player * m_player = nullptr;

	double m_elapsedTime = 0;
	double m_maxDuration = 20;
};