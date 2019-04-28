#include "pch.h"
#include "RedShell.h"
#include "AIScheduler.h"
#include "Player.h"


RedShell::RedShell() : Item(Locator::getItemData()->GetItemModelName(RED_SHELL))
{
	InitShellData();

	m_mesh->SetDrag(0);
	m_mesh->SetPhysicsOn(true);
	m_mesh->setDampenWallReflect(false);
	m_mesh->SetMaxSpeed(m_aiData.m_maxSpeed);

	m_move = std::make_unique<ControlledMovement>(m_mesh, m_displayedMesh.get());
	m_move->SetMoveSpeed(m_aiData.m_moveSpeed);
	m_move->SetTurnSpeed(m_aiData.m_turnSpeed);
}

void RedShell::InitShellData()
{
	m_aiData.m_maxSpeed = (float)m_itemData["RED_SHELL"]["info"]["max_speed"];
	m_usePosOffset = (float)m_itemData["RED_SHELL"]["info"]["use_pos_offset"];
	m_aiData.m_moveTowardDistSqrd = (float)m_itemData["RED_SHELL"]["info"]["move_toward_distance_squared"];
	m_aiData.m_moveSpeed = (float)m_itemData["RED_SHELL"]["info"]["ai"]["move_speed"];
	m_aiData.m_turnSpeed = (float)m_itemData["RED_SHELL"]["info"]["ai"]["turn_speed"];

	m_collisionData.m_playerVelMulti = (float)m_itemData["RED_SHELL"]["info"]["player_collision"]["velocity_multiplier"];
	m_collisionData.m_jumpHeight = (float)m_itemData["RED_SHELL"]["info"]["player_collision"]["jump"]["height"];
	m_collisionData.m_jumpDuration = (float)m_itemData["RED_SHELL"]["info"]["player_collision"]["jump"]["duration"];
	m_collisionData.m_flipRev = (float)m_itemData["RED_SHELL"]["info"]["player_collision"]["flip"]["revolutions"];
	m_collisionData.m_flipDuration = (float)m_itemData["RED_SHELL"]["info"]["player_collision"]["flip"]["duration"];
	m_collisionData.m_spinRev = (float)m_itemData["RED_SHELL"]["info"]["player_collision"]["spin"]["revolutions"];
	m_collisionData.m_spinDuration = (float)m_itemData["RED_SHELL"]["info"]["player_collision"]["spin"]["duration"];
	m_collisionData.m_vertPosOffset = (float)m_itemData["RED_SHELL"]["info"]["player_collision"]["vertical_pos_offset"];
}

void RedShell::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		if (!m_altUse)
		{
			m_move->Tick();
		}
	}
}

void RedShell::HitByPlayer(Player * _player)
{
	// Prevent collisions with the parent that fired this for a bit
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

void RedShell::Use(Player * _player, bool _altUse)
{
	setItemInUse(_player);
	
	m_mesh->SetWorld(_player->GetWorld());
	m_mesh->AddPos(_player->GetWorld().Right() * m_usePosOffset);
	m_mesh->SetOri(m_player->GetOri());
	m_mesh->UpdateWorld();
	Vector3 normVel = _player->getVelocity();
	normVel.Normalize();
	m_mesh->setVelocity(normVel * (_altUse? -m_aiData.m_moveSpeed : 1));

	if (!_altUse)
	{
		m_move->SetWaypoint(_player->GetWaypoint());
		m_ai = std::make_unique<MoveAI>(m_mesh, m_move.get());
		m_ai->SetAutoUpdateWaypoints(true);
	}

	m_altUse = _altUse;
}
