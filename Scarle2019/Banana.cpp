#include "pch.h"
#include "Banana.h"
#include "Player.h"
#include <iostream>

Banana::Banana() : Item("DEFAULT ITEM") {
	std::cout << "PROBABILITY FOR FIRST PLACE BANANA: " << m_item_config["probabilities"]["place_1"] << std::endl;
	SetDrag(0.9f);
	SetPhysicsOn(true);
}

void Banana::HitByPlayer(Player* player)
{
	player->setVelocity(player->getVelocity() / 3);
	m_shouldDestroy = true;
}