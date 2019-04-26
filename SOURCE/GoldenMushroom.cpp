#include "pch.h"
#include "GoldenMushroom.h"
#include "Player.h"
#include "GameStateData.h"

GoldenMushroom::GoldenMushroom()
{
	initItemData();
}

void GoldenMushroom::initItemData()
{
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	m_itemData << i;
	m_maxDuration = (float)m_itemData["MUSHROOM_UNLIMITED"]["info"]["lifetime"];
	m_boostData.m_boostAmount = (float)m_itemData["MUSHROOM_UNLIMITED"]["info"]["boost"]["amount"];
	m_boostData.m_minVelo = (float)m_itemData["MUSHROOM_UNLIMITED"]["info"]["boost"]["min_velocity"];
	m_boostData.m_boostDuration = (float)m_itemData["MUSHROOM_UNLIMITED"]["info"]["boost"]["duration"];
	m_boostData.m_playerDrag = (float)m_itemData["MUSHROOM_UNLIMITED"]["info"]["boost"]["player_drag"];
}

void GoldenMushroom::Tick()
{
	Item::Tick();

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
		}

	}

}

void GoldenMushroom::Use(Player * player, bool _altUse)
{
	setItemInUse(player);
	m_trailingPlayerImmunity = true;

	player->UseGroundTypes(false);
	player->SetDrag(0.7f);

	if (player->getVelocity().Length() < m_boostData.m_minVelo)
	{
		Vector vel = m_player->getVelocity();
		vel.Normalize();
		vel *= m_boostData.m_minVelo;
		m_player->setVelocity(vel);
	}


	m_boostData.m_boostTimeElapsed = 0;
}
