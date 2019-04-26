#include "pch.h"
#include "Banana.h"
#include "Player.h"
#include "ItemData.h"
#include <iostream>

Banana::Banana() : Item(Locator::getItemData()->GetItemModelName(BANANA))
{
	m_mesh->SetDrag(0.9f);
	m_mesh->SetPhysicsOn(true);

	InitItemData();
}

void Banana::InitItemData()
{
	m_throwData.m_thowHoriPosOffset = m_itemData["BANANA"]["info"]["throw"]["horizontal_pos_offset"];
	m_throwData.m_thowVertPosOffset = m_itemData["BANANA"]["info"]["throw"]["vertical_pos_offset"];
	m_throwData.m_forwardForce = m_itemData["BANANA"]["info"]["throw"]["forward_force"];
	m_throwData.m_upwardForce = m_itemData["BANANA"]["info"]["throw"]["upward_force"];
	m_throwData.m_maxGrav = m_itemData["BANANA"]["info"]["throw"]["max_gravity"];

	m_collisionData.m_playerVelMulti = (float)m_itemData["BANANA"]["info"]["player_collision"]["velocity_multiplier"];
	m_collisionData.m_jumpHeight = (float)m_itemData["BANANA"]["info"]["player_collision"]["jump"]["height"];
	m_collisionData.m_jumpDuration = (float)m_itemData["BANANA"]["info"]["player_collision"]["jump"]["duration"];
	m_collisionData.m_flipRev = (float)m_itemData["BANANA"]["info"]["player_collision"]["flip"]["revolutions"];
	m_collisionData.m_flipDuration = (float)m_itemData["BANANA"]["info"]["player_collision"]["flip"]["duration"];
	m_collisionData.m_spinRev = (float)m_itemData["BANANA"]["info"]["player_collision"]["spin"]["revolutions"];
	m_collisionData.m_spinDuration = (float)m_itemData["BANANA"]["info"]["player_collision"]["spin"]["duration"];
	m_collisionData.m_vertPosOffset = (float)m_itemData["BANANA"]["info"]["player_collision"]["vertical_pos_offset"];
}

void Banana::Use(Player * _player, bool _altUse)
{
	Item::setItemInUse(_player);

	if (_altUse)
	{
		m_mesh->SetWorld(_player->GetWorld());
		m_mesh->AddPos(_player->GetWorld().Right() * m_throwData.m_thowHoriPosOffset + _player->GetWorld().Up() * m_throwData.m_thowVertPosOffset);
		m_mesh->UpdateWorld();

		m_mesh->setMaxGrav(m_throwData.m_maxGrav);
		m_mesh->setGravVelocity(_player->getVelocity() + (_player->GetWorld().Forward() * m_throwData.m_forwardForce) + (_player->GetWorld().Up() * m_throwData.m_upwardForce));
	}
}

void Banana::HitByPlayer(Player* _player)
{
	if (Item::ignorePlayerCollisions(_player))
	{
		return;
	}

	_player->setVelocity(_player->getVelocity() * m_collisionData.m_playerVelMulti);
	_player->Jump(m_collisionData.m_jumpHeight, m_collisionData.m_jumpDuration);
	_player->Flip(m_collisionData.m_flipRev, m_collisionData.m_flipDuration);
	_player->Spin(m_collisionData.m_spinRev, m_collisionData.m_spinDuration);
	_player->AddPos(_player->GetWorld().Up() * m_collisionData.m_vertPosOffset);
	_player->UpdateWorld();
	m_shouldDestroy = true;
}