#include "pch.h"
#include "Star.h"

Star::Star()
{
	m_maxDuration = 12;
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

}
