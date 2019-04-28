#include "pch.h"
#include "BulletBill.h"
#include "Player.h"
#include <iostream>
#include <fstream>

BulletBill::BulletBill()
{
	InitBulletData();
}

void BulletBill::InitBulletData()
{
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	m_itemData << i;

	m_durationMulti = m_itemData["BULLET_BILL"]["info"]["duration_multiplier"];
	m_aiData.m_moveSpeed = m_itemData["BULLET_BILL"]["info"]["ai"]["player_move_speed"];
	m_aiData.m_turnSpeed = m_itemData["BULLET_BILL"]["info"]["ai"]["player_turn_speed"];
}

void BulletBill::Tick()
{
	if (m_itemUsed)
	{
		m_bulletTimeElapsed += Locator::getGSD()->m_dt;
		if (m_bulletTimeElapsed >= m_bulletDuration)
		{
			ControlledMovement* playerMovement = m_player->GetControlledMovement();
			playerMovement->SetMoveSpeed(m_playerMoveSpeed);
			playerMovement->SetTurnSpeed(m_playerTurnSpeed);

			m_player->GetMoveAi()->UseDrift(true);
			if (m_player->GetPlayerId() != -1)
			{
				playerMovement->SetGamepadActive(true);
				m_player->DeleteAi();
			}
			m_player->GetAnimController()->setLockSet(false);
			m_player->UseGroundTypes(true);
			m_player->SetActiveItem(BULLET_BILL);
			m_player->setInvicible(false);

			if (m_player->IsGliding())
			{
				m_player->GetAnimController()->SwitchModelSet("gliding");
			}
			else if (m_player->IsRespawning())
			{
				m_player->GetAnimController()->SwitchModelSet("respawn");
			}
			else
			{
				m_player->GetAnimController()->SwitchModelSet("default");
			}

			FlagForDestoy();
		}

	}
}

void BulletBill::Use(Player * player, bool _altUse)
{
	setItemInUse(player);
	m_trailingPlayerImmunity = true;

	player->GetAnimController()->SwitchModelSet("Bullet Bill");
	player->UseGroundTypes(false);
	player->GetAnimController()->setLockSet(true);
	m_bulletDuration = m_durationMulti * player->GetRanking();
	
	if (player->GetPlayerId() != -1)
	{
		player->CreateAi();
	}
	player->GetMoveAi()->UseDrift(false);
	player->setInvicible(true);

	ControlledMovement* playerMovement = player->GetControlledMovement();
	playerMovement->SetGamepadActive(false);
	m_playerMoveSpeed = playerMovement->GetMoveSpeed();
	m_playerTurnSpeed = playerMovement->GetTurnSpeed();

	playerMovement->SetMoveSpeed(m_aiData.m_moveSpeed);
	playerMovement->SetTurnSpeed(m_aiData.m_turnSpeed);
}
