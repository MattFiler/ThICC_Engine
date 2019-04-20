#include "pch.h"
#include "GoldenMushroom.h"
#include "Player.h"
#include "GameStateData.h"

GoldenMushroom::GoldenMushroom()
{
	m_maxDuration = 7.5;
}

void GoldenMushroom::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		Vector vel = m_player->getVelocity();
		vel.Normalize();
		vel *= m_boostAmount * Locator::getGSD()->m_dt;
		m_player->setVelocity(m_player->getVelocity() + vel);

		m_boostTimeElapsed += Locator::getGSD()->m_dt;
		if (m_boostTimeElapsed > m_boostDuration)
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

	if (player->getVelocity().Length() < m_minVelo)
	{
		Vector vel = m_player->getVelocity();
		vel.Normalize();
		vel *= m_minVelo;
		m_player->setVelocity(vel);
	}


	m_boostTimeElapsed = 0;
}
