#include "pch.h"
#include "Mushroom.h"
#include "ServiceLocator.h"

Mushroom::Mushroom()
{
}

void Mushroom::Use(Player* player)
{
	Vector vel = player->getVelocity();
	vel.Normalize();
	vel *= 30;
	player->setVelocity(player->getVelocity() + vel);
}

void Mushroom::HitByPlayer(Player* player)
{
}