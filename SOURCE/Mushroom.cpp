#include "pch.h"
#include "Mushroom.h"
#include "ServiceLocator.h"
#include "GameStateData.h"

Mushroom::Mushroom()
{
}

void Mushroom::Use(Player* player, bool _altUse)
{
	m_used = true;

	m_player = player;

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
	if (m_used)
	{
		Vector vel = m_player->getVelocity();
		vel.Normalize();
		vel *= m_boostAmount * Locator::getGSD()->m_dt;
		m_player->setVelocity(m_player->getVelocity() + vel);

		m_timeElapsed += Locator::getGSD()->m_dt;
		if (m_timeElapsed > m_boostDuration)
		{
			m_player->UseGroundTypes(true);
			m_shouldDestroy = true;
		}

	}

	Item::Tick();
}

void Mushroom::HitByPlayer(Player* player)
{
}