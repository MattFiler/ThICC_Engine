#include "pch.h"
#include "LightningCloud.h"
#include "Player.h"
#include "GameStateData.h"

LightningCloud::LightningCloud() : Item(Locator::getItemData()->GetItemModelName(LIGHTNING_CLOUD))
{
	m_shouldDespawn = false;
	m_growthData.m_scaleState = ItemGrowthData::SHRINK;

	initCloudData();
}

void LightningCloud::initCloudData()
{
	m_strikeDuration = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["strike_duration"];
	m_slowAmount = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["player_velocity_multiplier"];

	m_growthData.m_scaleMulti = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["shrink"]["size_multiplier"];
	m_growthData.m_growthSpeed = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["shrink"]["growth_speed"];
	m_growthData.m_shrinkSpeed = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["shrink"]["shrink_speed"];
	m_growthData.m_duration = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["shrink"]["max_shrink_duration"];
}

void LightningCloud::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		if (m_striked)
		{
			switch (m_growthData.m_scaleState)
			{
			case ItemGrowthData::GROW:
				m_player->SetScale(Vector3::Lerp(m_growthData.m_startScale, m_growthData.m_endScale, m_growthData.m_scalePercent));

				m_growthData.m_scalePercent -= m_growthData.m_growthSpeed * Locator::getGSD()->m_dt;

				if (m_growthData.m_scalePercent <= 0)
				{
					m_growthData.m_scalePercent = 0;
					m_shouldDestroy = true;
				}
				break;
			case ItemGrowthData::MAINTIAIN:
			{
				Vector vel = m_player->getVelocity();
				vel.Normalize();
				vel *= m_slowAmount * Locator::getGSD()->m_dt;
				m_player->setVelocity(m_player->getVelocity() + vel);

				m_growthData.m_timeElapsed += Locator::getGSD()->m_dt;
				if (m_growthData.m_timeElapsed >= m_growthData.m_duration)
				{
					m_growthData.m_scaleState = ItemGrowthData::GROW;
				}

				break;
			}
			case ItemGrowthData::SHRINK:
				m_player->SetScale(Vector3::Lerp(m_growthData.m_startScale, m_growthData.m_endScale, m_growthData.m_scalePercent));

				m_growthData.m_scalePercent += m_growthData.m_shrinkSpeed * Locator::getGSD()->m_dt;

				m_player->Spin(2, 0.7);

				if (m_growthData.m_scalePercent >= 1)
				{
					m_growthData.m_scalePercent = 1;
					m_growthData.m_scaleState = ItemGrowthData::MAINTIAIN;
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
				m_growthData.m_startScale = m_player->GetScale();
				m_growthData.m_endScale = m_growthData.m_startScale * m_growthData.m_scaleMulti;
				m_growthData.m_duration -= m_player->GetRanking();
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