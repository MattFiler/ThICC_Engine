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
};