#include "pch.h"
#include "Bomb.h"
#include "Player.h"

Bomb::Bomb(std::function<Explosion*()> _CreateExplosionFunciton) : Item(Locator::getItemData()->GetItemModelName(BOMB)), CreateExplosion(_CreateExplosionFunciton)
{
	m_mesh->SetDrag(0.9f);
	m_mesh->SetPhysicsOn(true);
}

void Bomb::Tick()
{
	Item::Tick();
}

void Bomb::Use(Player * player, bool _altUse)
{
	explosion = CreateExplosion();
	explosion->SetWorld(m_mesh->GetWorld());
}

void Bomb::HitByPlayer(Player * player)
{
	CreateExplosion();
}
