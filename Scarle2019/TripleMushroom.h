#pragma once

#include "Item.h"
#include "Mushroom.h"
class TripleMushroom : public Item
{
public:
	TripleMushroom();
	~TripleMushroom();

	virtual void Tick() override;
	void HitByPlayer(Player* player) override;
	void Use(Player* player, bool _altUse) override;

private:
	std::queue<Mushroom*> m_mushrooms;
	Mushroom* m_currentMushroom = nullptr;
};

