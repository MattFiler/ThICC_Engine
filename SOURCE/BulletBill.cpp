#include "pch.h"
#include "BulletBill.h"
#include "Player.h"


BulletBill::BulletBill()
{
	m_aiData.m_moveSpeed = 50;
	m_aiData.m_turnSpeed = 50;
}

void BulletBill::Tick()
{
	if (m_itemUsed)
	{
		m_player->GetControlledMovement()->Tick();

		if (m_elapsedTime >= m_maxDuration)
		{
			ControlledMovement* playerMovement = m_player->GetControlledMovement();
			playerMovement->SetMoveSpeed(m_playerMoveSpeed);
			playerMovement->SetTurnSpeed(m_playerTurnSpeed);
			if (m_player->GetPlayerId() != -1)
			{
				m_player->DeleteAi();
			}
		}

	}
	Item::Tick();
}

void BulletBill::Use(Player * player, bool _altUse)
{
	setItemInUse(player);
	m_trailingPlayerImmunity = true;

	player->GetAnimController()->SwitchModelSet("Bullet Bill");
	player->GetAnimController()->setLockSet(true);
	m_maxDuration = m_durationMulti * player->GetRanking();
	
	if (player->GetPlayerId() != -1)
	{
		player->CreateAi();
	}
	ControlledMovement* playerMovement = player->GetControlledMovement();
	m_playerMoveSpeed = playerMovement->GetMoveSpeed();
	m_playerTurnSpeed = playerMovement->GetTurnSpeed();

	playerMovement->SetMoveSpeed(m_aiData.m_moveSpeed);
	playerMovement->SetTurnSpeed(m_aiData.m_turnSpeed);
}
