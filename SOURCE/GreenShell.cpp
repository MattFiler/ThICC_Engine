#include "pch.h"
#include "GreenShell.h"
#include "Player.h"
#include "ServiceLocator.h"

GreenShell::GreenShell() : Item(GREEN_SHELL)
{
	m_itemMesh->m_mesh->SetDrag(0);
	m_itemMesh->m_mesh->SetPhysicsOn(true);
	m_itemMesh->m_mesh->setDampenWallReflect(false);

	InitShellData();
	m_itemMesh->m_displayedMesh->ResetRotation();

}

void GreenShell::InitShellData()
{
	m_speed = (float)m_itemData["GREEN_SHELL"]["info"]["speed"];
	m_usePosOffset = (float)m_itemData["GREEN_SHELL"]["info"]["use_pos_offset"];
	m_maxBounceCount = m_itemData["GREEN_SHELL"]["info"]["max_bounce_count"];
	m_frameDelay = m_itemData["GREEN_SHELL"]["info"]["collision_frame_delay"];

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
	m_itemMesh->m_displayedMesh->ResetRotation();
	m_shouldDestroy = true;
}

void GreenShell::Use(Player * player, bool _altUse)
{
	Item::setItemInUse(player);

	m_itemMesh->m_mesh->SetWorld(player->GetWorld());
	m_itemMesh->m_mesh->AddPos(player->GetWorld().Right() * m_usePosOffset);
	m_itemMesh->m_mesh->UpdateWorld();
	m_itemMesh->m_mesh->setVelocity(m_speed * player->GetWorld().Forward() * (_altUse ? -1 : 1));
}

void GreenShell::Tick()
{
	if (m_itemUsed)
	{
		m_itemMesh->m_displayedMesh->Spin(m_spinRev, m_spinDuration);
		
		++m_frameCount;

		if (m_itemMesh->m_mesh->HasHitWall() && m_frameCount >= m_frameDelay)
		{
			++m_bounceCount;
			m_itemMesh->m_mesh->ResetHasHitWall();
			m_frameCount = 0;
		}
		m_shouldDestroy = m_bounceCount >= m_maxBounceCount;
	}
	Item::Tick();
}