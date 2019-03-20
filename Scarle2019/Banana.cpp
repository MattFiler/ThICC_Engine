#include "pch.h"
#include "Banana.h"
#include "Player.h"
#include <iostream>

Banana::Banana() : Item(Locator::getItemData()->GetItemModelName(BANANA)) {
	m_mesh->SetDrag(0.9f);
	m_mesh->SetPhysicsOn(true);
}

void Banana::HitByPlayer(Player* player)
{
	player->setVelocity(player->getVelocity() / 3);
	m_shouldDestroy = true;
}