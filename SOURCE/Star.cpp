#include "pch.h"
#include "Star.h"
#include "Player.h"
#include "GameStateData.h"

Star::Star()
{
	InitStarData();
}

void Star::InitStarData()
{
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	m_itemData << i;
	m_maxDuration = (float)m_itemData["STAR"]["info"]["duration"];
	m_boostAmount = (float)m_itemData["STAR"]["info"]["boost"]["amount"];
	m_playerDrag = (float)m_itemData["STAR"]["info"]["boost"]["player_drag"];
}

void Star::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		m_player->setInvicible(m_elapsedTime < m_maxDuration);
		m_player->UseGroundTypes(!(m_elapsedTime < m_maxDuration));

		if (m_elapsedTime < m_maxDuration)
		{
			Vector vel = m_player->getVelocity();
			vel.Normalize();
			vel *= m_boostAmount * Locator::getGSD()->m_dt;
			m_player->setVelocity(m_player->getVelocity() + vel);
		}
	}
}

void Star::Use(Player * player, bool _altUse)
{
	setItemInUse(player);
	m_trailingPlayerImmunity = true;
	player->SetDrag(m_playerDrag);
}
