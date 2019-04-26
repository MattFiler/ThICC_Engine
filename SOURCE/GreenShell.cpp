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

	m_playerVel = Vector3((float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["velocity"][0],
		(float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["velocity"][1],
		(float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["velocity"][2]);
	m_playerJumpHeight = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["jump"]["height"];
	m_playerJumpDuration = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["jump"]["duration"];
	m_playerFlipRev = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["flip"]["revolutions"];
	m_playerFlipDuration = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["flip"]["duration"];
	m_playerVerticalPosOffset = (float)m_itemData["GREEN_SHELL"]["info"]["player_collision"]["vertical_pos_offset"];
}

void GreenShell::HitByPlayer(Player* player)
{
	// Prevent collisions with the parent that fired this for a bit
	if (Item::ignorePlayerCollisions(player))
	{
		return;
	}

	player->setVelocity(m_playerVel);
	player->Jump(m_playerJumpHeight, m_playerJumpDuration);
	player->Flip(m_playerFlipRev, m_playerFlipDuration);
	player->AddPos(player->GetWorld().Up() * m_playerVerticalPosOffset);
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