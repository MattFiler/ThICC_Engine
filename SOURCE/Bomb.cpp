#include "pch.h"
#include "Bomb.h"
#include "Player.h"
#include "Explosion.h"
#include "GameStateData.h"

Bomb::Bomb(std::function<Explosion*()> _CreateExplosionFunciton) : Item(Locator::getItemData()->GetItemModelName(BOMB)), CreateExplosion(_CreateExplosionFunciton)
{
	m_mesh->SetDrag(0.9f);
	m_mesh->SetPhysicsOn(true);
	m_maxImmunityTime = 1;
	m_maxImmunityTime = 0.1;

}

void Bomb::Tick()
{
	Item::Tick();

	if (m_countdown)
	{
		m_current_time += Locator::getGSD()->m_dt;

		if (m_current_time >= m_countdown_time)
		{
			Detonate();
		}
	}
}

void Bomb::Use(Player * player, bool _altUse)
{
	Item::setItemInUse(player);

	if (_altUse)
	{
		//Positions the bomb
		m_mesh->SetWorld(player->GetWorld());
		m_mesh->AddPos(player->GetWorld().Right() * 2 + player->GetWorld().Up() * 3);
		m_mesh->UpdateWorld();

		//Yeets said bomb
		float forward_force = 100;
		float upward_force = 1;
		m_mesh->setMaxGrav(500);
		m_mesh->setGravVelocity(player->getVelocity() + (player->GetWorld().Forward() * forward_force) + (player->GetWorld().Up() * upward_force));
	}
	m_countdown = true;
	//Detonate();
}

void Bomb::Detonate()
{
	m_countdown = false;
	explosion = CreateExplosion();
	explosion->SetWorld(m_mesh->GetWorld());
	explosion->explode();
	FlagForDestoy();
}

void Bomb::HitByPlayer(Player * player)
{
	if (Item::ignorePlayerCollisions(player))
	{
		return;
	}
	Detonate();
}
