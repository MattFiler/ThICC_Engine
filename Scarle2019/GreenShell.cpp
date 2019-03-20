#include "pch.h"
#include "GreenShell.h"
#include "Player.h"
#include "ServiceLocator.h"
#include <iostream>

GreenShell::GreenShell() : Item(Locator::getItemData()->GetItemModelName(GREEN_SHELL))
{
	SetDrag(0);
	SetPhysicsOn(true);
	setDampenWallReflect(false);
}

void GreenShell::HitByPlayer(Player* player)
{
	player->setVelocity(Vector3::Zero);
	player->AddPos(player->GetWorld().Up() * 4);
	m_shouldDestroy = true;
}

void GreenShell::Tick()
{
	m_elapsedTime += Locator::getGSD()->m_dt;
	if (m_elapsedTime > m_maxDuration)
	{
		m_shouldDestroy = true;
	}
	Item::Tick();
}