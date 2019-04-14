#pragma once
#include "Item.h"

class FakeItemBox : public Item
{
public:
	FakeItemBox();
	~FakeItemBox() = default;

	virtual void Tick() override { Item::Tick(); };

	void Use(Player* player, bool _altUse) override;
	void HitByPlayer(Player* player) override;
};