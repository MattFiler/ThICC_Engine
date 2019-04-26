#include "pch.h"
#include "Mushroom.h"
#include "ServiceLocator.h"
#include "Player.h"
#include "GameStateData.h"


Mushroom::Mushroom()
{
	InitMushroomData();
	m_shouldDespawn = false;
}

void Mushroom::InitMushroomData()
{
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	m_itemData << i;
	m_boostData.m_boostAmount = (float)m_itemData["MUSHROOM"]["info"]["boost"]["amount"];
	m_boostData.m_minVelo = (float)m_itemData["MUSHROOM"]["info"]["boost"]["min_velocity"];
	m_boostData.m_boostDuration = (float)m_itemData["MUSHROOM"]["info"]["boost"]["duration"];
	m_boostData.m_playerDrag = (float)m_itemData["MUSHROOM"]["info"]["boost"]["player_drag"];
}

void Mushroom::Use(Player* player, bool _altUse)
{
	setItemInUse(player);
	m_trailingPlayerImmunity = true;

	player->UseGroundTypes(false);
	player->SetDrag(m_boostData.m_playerDrag);

	if (player->getVelocity().Length() < m_boostData.m_minVelo)
	{
		Vector vel = m_player->getVelocity();
		vel.Normalize();
		vel *= m_boostData.m_minVelo;
		m_player->setVelocity(vel);
	}
}

void Mushroom::Tick()
{
	if (m_itemUsed)
	{
		Vector vel = m_player->getVelocity();
		vel.Normalize();
		vel *= m_boostData.m_boostAmount * Locator::getGSD()->m_dt;
		m_player->setVelocity(m_player->getVelocity() + vel);

		m_boostData.m_boostTimeElapsed += Locator::getGSD()->m_dt;
		if (m_boostData.m_boostTimeElapsed > m_boostData.m_boostDuration)
		{
			m_player->UseGroundTypes(true);
			m_shouldDestroy = true;
		}

	}

	Item::Tick();
}