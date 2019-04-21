#include "pch.h"
#include "GreenShell.h"
#include "Player.h"
#include "ServiceLocator.h"

GreenShell::GreenShell() : Item(Locator::getItemData()->GetItemModelName(BLUE_SHELL))
{
	m_mesh->SetDrag(0);
	m_mesh->SetPhysicsOn(true);
	m_mesh->setDampenWallReflect(false);

	m_maxDuration = 20;
	m_maxImmunityTime = 0.5;

}

void GreenShell::HitByPlayer(Player* player)
{
	// Prevent collisions with the parent that fired this for a bit
	if (Item::ignorePlayerCollisions(player))
	{
		return;
	}

	player->setVelocity(Vector3::Zero);
	player->Jump(1.5f, 1);
	player->Flip(1, 0.8f);
	player->AddPos(player->GetWorld().Up() * 4);
	m_shouldDestroy = true;
}

void GreenShell::Use(Player * player, bool _altUse)
{
	Item::setItemInUse(player);

	m_mesh->SetWorld(player->GetWorld());
	m_mesh->AddPos(player->GetWorld().Right() * 1);
	m_mesh->UpdateWorld();
	m_mesh->setVelocity(60 * player->GetWorld().Forward() * (_altUse ? -1 : 1));
}

void GreenShell::Tick()
{
	if (m_itemUsed)
	{
		m_displayedMesh->Spin(1, 0.7);
	}
	Item::Tick();
}