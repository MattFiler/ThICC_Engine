#include "pch.h"
#include "Banana.h"
#include "Player.h"
#include <iostream>

Banana::Banana() : Item(Locator::getItemData()->GetItemModelName(BANANA)) 
{
	m_mesh->SetDrag(0.9f);
	m_mesh->SetPhysicsOn(true);
}

void Banana::Use(Player * _player, bool _altUse)
{
	if (_altUse)
	{
		//Positions the banan
		m_mesh->SetWorld(_player->GetWorld());
		m_mesh->AddPos(_player->GetWorld().Right() * 2 + _player->GetWorld().Up() * 2);
		m_mesh->UpdateWorld();

		//Yeets said banan
		float forward_force = 100;
		float upward_force = 1;
		m_mesh->setMaxGrav(500);
		m_mesh->setGravVelocity(_player->getVelocity() + (_player->GetWorld().Forward() * forward_force) + (_player->GetWorld().Up() * upward_force));
	}
}

void Banana::HitByPlayer(Player* _player)
{
	_player->setVelocity(_player->getVelocity() / 3);
	m_shouldDestroy = true;
}