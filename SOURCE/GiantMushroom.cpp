#include "pch.h"
#include "GiantMushroom.h"
#include "Player.h"
#include "GameStateData.h"

void GiantMushroom::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		switch (m_scaleState)
		{
		case GiantMushroom::GROW:
			m_player->SetScale(Vector3::Lerp(m_startScale, m_endScale, m_scalePercent));
			m_player->UpdateWorld();

			m_scalePercent += m_scaleSpeed * Locator::getGSD()->m_dt;

			if (m_scalePercent >= 1)
			{
				m_scalePercent = 1;
				m_scaleState = MAINTIAIN;
			}
			break;

		case GiantMushroom::MAINTIAIN:

			m_player->setInvicible(m_growthTimeElapsed < m_maxGrowthTime);
			m_growthTimeElapsed += Locator::getGSD()->m_dt;
			if (m_growthTimeElapsed >= m_maxGrowthTime)
			{
				m_scaleState = SHRINK;
			}
			break;

		case GiantMushroom::SHRINK:
			m_player->setInvicible(false);
			m_player->SetScale(Vector3::Lerp(m_startScale, m_endScale, m_scalePercent));
			m_player->UpdateWorld();

			m_scalePercent -= m_scaleSpeed * 1.5 * Locator::getGSD()->m_dt;

			if (m_scalePercent <= 0)
			{
				m_scalePercent = 0;
				m_shouldDestroy = true;
			}
			break;
		}
	}
}

void GiantMushroom::Use(Player * player, bool _altUse)
{
	if (!player->isInvincible())
	{
		setItemInUse(player);
		m_trailingPlayerImmunity = true;
		m_startScale = m_player->GetScale();
		m_endScale = m_startScale * 2;
	}
}

