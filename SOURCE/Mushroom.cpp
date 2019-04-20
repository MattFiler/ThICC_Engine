#include "pch.h"
#include "Mushroom.h"
#include "ServiceLocator.h"
#include "Player.h"
#include "GameStateData.h"


Mushroom::Mushroom()
{
}

void Mushroom::Use(Player* player, bool _altUse)
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
}

void Mushroom::Tick()
{
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
			m_shouldDestroy = true;
		}

	}

	Item::Tick();
}