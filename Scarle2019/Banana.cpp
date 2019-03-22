#include "pch.h"
#include "Banana.h"
#include "Player.h"
#include <iostream>

Banana::Banana() : Item(Locator::getItemData()->GetItemModelName(BANANA)) 
{
	m_mesh->SetDrag(0.9f);
	m_mesh->SetPhysicsOn(true);
}

void Banana::Use(Player * player, bool _altUse)
{
	if (_altUse)
	{
		m_mesh->SetWorld(player->GetWorld());
		m_mesh->AddPos(player->GetWorld().Right() * 3);
		m_mesh->UpdateWorld();

		float throw_force = 500;

		Vector3 forward = player->GetWorld().Forward();
		Vector3 up = player->GetWorld().Up();

		m_mesh->setVelocity(player->GetWorld().Forward() * player->GetWorld().Up() * throw_force);
	}
}

void Banana::HitByPlayer(Player* player)
{
	player->setVelocity(player->getVelocity() / 3);
	m_shouldDestroy = true;
}