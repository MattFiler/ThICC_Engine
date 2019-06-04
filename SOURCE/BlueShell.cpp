#include "pch.h"
#include "BlueShell.h"


BlueShell::BlueShell(std::function<Explosion*(ItemType)> _CreateExplosionFunciton, std::vector<Player*> _players) : CreateExplosion(_CreateExplosionFunciton), m_players(_players)
, Item(BLUE_SHELL)
{
	InitShellData();

	m_itemMesh->m_mesh->SetDrag(0);
	m_itemMesh->m_mesh->SetPhysicsOn(true);
	m_itemMesh->m_mesh->setDampenWallReflect(false);
	m_itemMesh->m_mesh->SetMaxSpeed(m_aiData.m_maxSpeed);
	m_itemMesh->m_displayedMesh->SetLockPosOffSet(true);

	m_move = std::make_unique<ControlledMovement>(m_itemMesh->m_mesh.get(), m_itemMesh->m_displayedMesh.get());
	m_move->SetMoveSpeed(m_aiData.m_moveSpeed);
	m_move->SetTurnSpeed(m_aiData.m_turnSpeed);
}

void BlueShell::InitShellData()
{
	m_aiData.m_maxSpeed = (float)m_itemData["BLUE_SHELL"]["info"]["max_speed"];
	m_usePosOffset = (float)m_itemData["BLUE_SHELL"]["info"]["use_pos_offset"];
	m_aiData.m_moveTowardDistSqrd = (float)m_itemData["BLUE_SHELL"]["info"]["move_toward_distance_squared"];
	m_aiData.m_moveSpeed = (float)m_itemData["BLUE_SHELL"]["info"]["ai"]["move_speed"];
	m_aiData.m_turnSpeed = (float)m_itemData["BLUE_SHELL"]["info"]["ai"]["turn_speed"];
}

void BlueShell::Tick()
{
	if (m_itemUsed)
	{
		Item::Tick();

		switch (m_moveState)
		{
		case UP:
			m_itemMesh->m_displayedMesh->SetPosOffSet(Vector3::Lerp(m_posOffset, m_airHeight * m_itemMesh->m_mesh->GetWorld().Up() + m_velocity, m_lerpPercent));
			m_lerpPercent += m_lerpSpeed * Locator::getGSD()->m_dt;

			if (m_lerpPercent >= 1)
			{
				m_moveState = ACROSS;
			}

			break;

		case ACROSS:
			m_move->Tick();

			if (Vector3::DistanceSquared(m_itemMesh->m_mesh->GetPos(), m_targetPlayer->GetPos()) < m_aiData.m_moveTowardDistSqrd)
			{
				if (m_ai)
				{
					m_ai.reset();
				}
				Vector3 moveTowards = m_targetPlayer->GetPos() - m_itemMesh->m_mesh->GetPos();
				moveTowards.Normalize();
				m_itemMesh->m_mesh->setVelocity(moveTowards * m_move->GetMoveSpeed());
			}
			break;

		case DOWN:
			m_itemMesh->m_displayedMesh->SetPosOffSet(Vector3::Lerp(m_posOffset, m_airHeight * m_itemMesh->m_mesh->GetWorld().Up() + m_velocity, m_lerpPercent));
			m_lerpPercent -= m_lerpSpeed * Locator::getGSD()->m_dt;

			if (m_lerpPercent <= 0)
			{
				m_explosion = CreateExplosion(BLUE_SHELL);
				m_explosion->SetWorld(m_itemMesh->m_mesh->GetWorld());
				m_explosion->Explode();
				FlagForDestoy();
			}
			break;
		}
	}
}

void BlueShell::HitByPlayer(Player * _player)
{
	if (_player == m_targetPlayer && m_moveState == ACROSS)
	{
		m_moveState = DOWN;
	}
}

void BlueShell::Use(Player * _player, bool _altUse)
{
	setItemInUse(_player);

	m_itemMesh->m_mesh->SetWorld(_player->GetWorld());
	m_itemMesh->m_mesh->AddPos(_player->GetWorld().Right() * m_usePosOffset);
	m_itemMesh->m_mesh->AddPos(_player->GetWorld().Up() * m_usePosOffset * 2);
	m_itemMesh->m_mesh->SetOri(m_player->GetOri());
	m_itemMesh->m_mesh->UpdateWorld();

	m_itemMesh->m_displayedMesh->GetModelFromSet(m_itemMesh->m_displayedMesh->GetCurrentSet(), "item")->GetModel()->SetOri(m_player->GetOri());
	m_itemMesh->m_displayedMesh->GetModelFromSet(m_itemMesh->m_displayedMesh->GetCurrentSet(), "item")->GetModel()->UpdateWorld();

	m_posOffset = m_itemMesh->m_displayedMesh->GetPosOffset();
	m_velocity = m_player->getVelocity() * 0.2f;

	Vector3 normVel = _player->getVelocity();
	normVel.Normalize();
	m_itemMesh->m_mesh->setVelocity(normVel);
	m_move->SetWaypoint(_player->GetWaypoint());
	m_ai = std::make_unique<MoveAI>(m_itemMesh->m_mesh.get(), m_move.get());
	m_ai->SetAutoUpdateWaypoints(true);

	FindTargetPlayer();
}

void BlueShell::FindTargetPlayer()
{
	for (Player* player : m_players)
	{
		if (m_targetPlayer)
		{
			if (player->GetRanking() < m_targetPlayer->GetRanking())
			{
				m_targetPlayer = player;
			}
		}
		else
		{
			m_targetPlayer = player;
		}
	}

	m_players.clear();
}

