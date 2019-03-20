#include "pch.h"
#include "GreenShell.h"
#include "Player.h"
#include "ServiceLocator.h"

GreenShell::GreenShell() : Item(Locator::getItemData()->GetItemModelName(GREEN_SHELL))
{
	m_mesh->SetDrag(0);
	m_mesh->SetPhysicsOn(true);
	m_mesh->setDampenWallReflect(false);
}

void GreenShell::HitByPlayer(Player* player)
{
	player->setVelocity(Vector3::Zero);
	player->AddPos(player->GetWorld().Up() * 4);
	m_shouldDestroy = true;
}

void GreenShell::Use(Player * player)
{
	m_mesh->SetWorld(player->GetWorld());
	m_mesh->AddPos(player->GetWorld().Forward() * 3);
	m_mesh->UpdateWorld();
	m_mesh->setVelocity(60 * player->GetWorld().Forward());
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