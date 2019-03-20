#include "pch.h"
#include "Banana.h"
#include "Player.h"
#include <iostream>

Banana::Banana() : Item(Locator::getItemData()->GetItemModelName(BANANA)) {
	SetDrag(0.9f);
	SetPhysicsOn(true);
}

void Banana::HitByPlayer(Player* player)
{
	player->setVelocity(player->getVelocity() / 3);
	m_shouldDestroy = true;
}