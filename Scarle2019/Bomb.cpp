#include "pch.h"
#include "Bomb.h"
#include "Player.h"

Bomb::Bomb() : Item(Locator::getItemData()->GetItemModelName(BOMB))
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
}

void Bomb::HitByPlayer(Player * player)
{
}
