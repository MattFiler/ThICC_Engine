#include "pch.h"
#include "LightningCloud.h"
#include "Player.h"
#include "GameStateData.h"

LightningCloud::LightningCloud() : Item(Locator::getItemData()->GetItemModelName(LIGHTNING_CLOUD))
{}

void LightningCloud::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		if (m_striked)
		{
			switch (m_scaleState)
			{
			case GROW:
				m_player->SetScale(Vector3::Lerp(m_startScale, m_endScale, m_scalePercent));

				m_scalePercent -= m_scaleSpeed * Locator::getGSD()->m_dt;

				if (m_scalePercent <= 0)
				{
					m_scalePercent = 0;
					m_shouldDestroy = true;
				}
				break;
			case MAINTIAIN:
			{
				Vector vel = m_player->getVelocity();
				vel.Normalize();
				vel *= m_slowAmount * Locator::getGSD()->m_dt;
				m_player->setVelocity(m_player->getVelocity() + vel);

				m_shrinkTimeElapsed += Locator::getGSD()->m_dt;
				if (m_shrinkTimeElapsed >= m_shrinkDuration)
				{
					m_scaleState = GROW;
				}

				break;
			}
			case SHRINK:
				m_player->SetScale(Vector3::Lerp(m_startScale, m_endScale, m_scalePercent));

				m_scalePercent += m_scaleSpeed * Locator::getGSD()->m_dt;

				m_player->Spin(2, 0.7);

				if (m_scalePercent >= 1)
				{
					m_scalePercent = 1;
					m_scaleState = MAINTIAIN;
				}
				break;
			}
		}
		else
		{
			m_strikeTimeElapsed += Locator::getGSD()->m_dt;
			if (m_strikeTimeElapsed >= m_strikeDuration)
			{
				m_striked = true;
				m_startScale = m_player->GetScale();
				m_endScale = m_startScale / 2;
				m_shrinkDuration -= m_player->GetRanking();
				m_displayedMesh->SetShouldRender(false);
			}
		}
	}
}

void LightningCloud::Use(Player * player, bool _altUse)
{
	if (player->isInvincible() || player->GetLightningCloud())
	{
		m_shouldDestroy = false;
		return;
	}

	setItemInUse(player);
	m_trailingPlayerImmunity = true;
}