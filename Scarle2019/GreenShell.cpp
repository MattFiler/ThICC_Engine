#include "pch.h"
#include "GreenShell.h"
#include "Player.h"
#include "ServiceLocator.h"

GreenShell::GreenShell() : Item(Locator::getItemData()->GetItemModelName(GREEN_SHELL))
{
	m_mesh->SetDrag(0);
	m_mesh->SetPhysicsOn(true);
	m_mesh->setDampenWallReflect(false);

	m_displayedMesh->Spin(1000, 300);
}

void GreenShell::HitByPlayer(Player* player)
{
	// Prevent collisions with the parent that fired this for a bit
	if (player == m_player)
	{
		return;
	}
	player->Jump(1.5f, 1);
	player->Flip(1, 0.8f);
	player->AddPos(player->GetWorld().Up() * 4);
	m_shouldDestroy = true;
}

void GreenShell::Use(Player * player, bool _altUse)
{
	m_player = player;
	m_mesh->SetWorld(player->GetWorld());
	m_mesh->AddPos(player->GetWorld().Right() * 1);
	m_mesh->UpdateWorld();
	m_mesh->setVelocity(60 * player->GetWorld().Forward() * (_altUse? -1 : 1));
}

void GreenShell::Tick()
{
	if (m_player)
	{
		m_elapsedTime += Locator::getGSD()->m_dt;
		if (m_elapsedTime > m_maxDuration)
		{
			m_shouldDestroy = true;
		}
	}
	Item::Tick();
}