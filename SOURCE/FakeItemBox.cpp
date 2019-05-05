#include "pch.h"
#include "FakeItemBox.h"
#include "Player.h"

FakeItemBox::FakeItemBox() : Item(FAKE_BOX)
{
	m_itemMesh->m_mesh->SetDrag(0.9f);
	m_itemMesh->m_mesh->SetPhysicsOn(true);

	InitBoxData();
}

void FakeItemBox::InitBoxData()
{
	m_throwData.m_thowHoriPosOffset = m_itemData["FAKE_BOX"]["info"]["throw"]["horizontal_pos_offset"];
	m_throwData.m_thowVertPosOffset = m_itemData["FAKE_BOX"]["info"]["throw"]["vertical_pos_offset"];
	m_throwData.m_forwardForce = m_itemData["FAKE_BOX"]["info"]["throw"]["forward_force"];
	m_throwData.m_upwardForce = m_itemData["FAKE_BOX"]["info"]["throw"]["upward_force"];
	m_throwData.m_maxGrav = m_itemData["FAKE_BOX"]["info"]["throw"]["max_gravity"];

	m_collisionData.m_playerVelMulti = (float)m_itemData["FAKE_BOX"]["info"]["player_collision"]["velocity_multiplier"];
	m_collisionData.m_jumpHeight = (float)m_itemData["FAKE_BOX"]["info"]["player_collision"]["jump"]["height"];
	m_collisionData.m_jumpDuration = (float)m_itemData["FAKE_BOX"]["info"]["player_collision"]["jump"]["duration"];
	m_collisionData.m_flipRev = (float)m_itemData["FAKE_BOX"]["info"]["player_collision"]["flip"]["revolutions"];
	m_collisionData.m_flipDuration = (float)m_itemData["FAKE_BOX"]["info"]["player_collision"]["flip"]["duration"];
	m_collisionData.m_spinRev = (float)m_itemData["FAKE_BOX"]["info"]["player_collision"]["spin"]["revolutions"];
	m_collisionData.m_spinDuration = (float)m_itemData["FAKE_BOX"]["info"]["player_collision"]["spin"]["duration"];
	m_collisionData.m_vertPosOffset = (float)m_itemData["FAKE_BOX"]["info"]["player_collision"]["vertical_pos_offset"];
}

void FakeItemBox::Use(Player * player, bool _altUse)
{
	Item::setItemInUse(player);

	if (_altUse)
	{
		m_itemMesh->m_mesh->SetWorld(player->GetWorld());
		m_itemMesh->m_mesh->AddPos(player->GetWorld().Right() * m_throwData.m_thowHoriPosOffset + player->GetWorld().Up() * m_throwData.m_thowVertPosOffset);
		m_itemMesh->m_mesh->UpdateWorld();

		m_itemMesh->m_mesh->setMaxGrav(m_throwData.m_maxGrav);
		m_itemMesh->m_mesh->setGravVelocity(player->getVelocity() + (player->GetWorld().Forward() * m_throwData.m_forwardForce) + (player->GetWorld().Up() * m_throwData.m_upwardForce));
	}
}

void FakeItemBox::HitByPlayer(Player * player)
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
