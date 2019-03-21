#include "pch.h"
#include "Mushroom.h"
#include "ServiceLocator.h"

Mushroom::Mushroom()
{
}

void Mushroom::Use(Player* player)
{
	m_used = true;
	m_player = player;
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
			m_shouldDestroy = true;
		}

	}

	Item::Tick();
}

void Mushroom::HitByPlayer(Player* player)
{
}