#include "pch.h"
#include "GreenShell.h"
#include "Player.h"
#include "ServiceLocator.h"

GreenShell::GreenShell() : Item(Locator::getItemData()->GetItemModelName(GREEN_SHELL))
{
	m_mesh->SetDrag(0);
	m_mesh->SetPhysicsOn(true);
	m_mesh->setDampenWallReflect(false);

	initItemData();
}

void GreenShell::initItemData()
{
	m_speed = (float)m_itemData["GREEN_SHELL"]["info"]["speed"];
	m_usePosOffset = (float)m_itemData["GREEN_SHELL"]["info"]["use_pos_offset"];

	m_spinRev = (float)m_itemData["GREEN_SHELL"]["info"]["spin"]["revolutions"];
	m_spinDuration = m_itemData["GREEN_SHELL"]["info"]["spin"]["duration"];

	m_collisionData.m_playerVelMulti = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["velocity_multiplier"];
	m_collisionData.m_jumpHeight = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["jump"]["height"];
	m_collisionData.m_jumpDuration = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["jump"]["duration"];
	m_collisionData.m_flipRev = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["flip"]["revolutions"];
	m_collisionData.m_flipDuration = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["flip"]["duration"];
	m_collisionData.m_spinRev = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["spin"]["revolutions"];
	m_collisionData.m_spinDuration = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["spin"]["duration"];
	m_collisionData.m_vertPosOffset = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["vertical_pos_offset"];
}

void GreenShell::HitByPlayer(Player* player)
{
	// Prevent collisions with the parent that fired this for a bit
	if (Item::ignorePlayerCollisions(player))
	{
		return;
	}

	player->setVelocity(player->getVelocity() * m_collisionData.m_playerVelMulti);
	player->Jump(m_collisionData.m_jumpHeight, m_collisionData.m_jumpDuration);
	player->Flip(m_collisionData.m_flipRev, m_collisionData.m_flipDuration);
	player->Spin(m_collisionData.m_spinRev, m_collisionData.m_spinDuration);
	player->AddPos(player->GetWorld().Up() * m_collisionData.m_vertPosOffset);
	player->UpdateWorld();
	m_shouldDestroy = true;
}

void GreenShell::Use(Player * player, bool _altUse)
{
	Item::setItemInUse(player);

	m_mesh->SetWorld(player->GetWorld());
	m_mesh->AddPos(player->GetWorld().Right() * m_usePosOffset);
	m_mesh->UpdateWorld();
	m_mesh->setVelocity(m_speed * player->GetWorld().Forward() * (_altUse ? -1 : 1));
}

void GreenShell::Tick()
{
	if (m_itemUsed)
	{
		m_displayedMesh->Spin(m_spinRev, m_spinDuration);
	}
	Item::Tick();
}