#include "pch.h"
#include "LightningCloud.h"
#include "Player.h"
#include "GameStateData.h"
#include "AudioManager.h"

LightningCloud::LightningCloud() : Item(LIGHTNING_CLOUD)
{
	m_shouldDespawn = false;
	m_growthData.m_scaleState = ItemGrowthData::SHRINK;

	initCloudData();
}

void LightningCloud::initCloudData()
{
	m_strikeDuration = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["strike_duration"];
	m_slowAmount = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["player_velocity_multiplier"];
	m_maxSpeedMutli = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["player_max_velocity_multiplier"];
	m_playerSpinRev = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["player_spin"]["revolutions"];
	m_playerSpinDuration = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["player_spin"]["duration"];

	m_growthData.m_scaleMulti = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["shrink"]["size_multiplier"];
	m_growthData.m_growthDuration = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["shrink"]["growth_duration"];
	m_growthData.m_shrinkDuration = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["shrink"]["shrink_duraion"];
	m_growthData.m_sizeChangeDuration = (float)m_itemData["LIGHTNING_CLOUD"]["info"]["shrink"]["max_shrink_duration"];
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
				if (!m_growthData.m_growing)
				{
					m_player->Scale(m_growthData.m_growScale, m_growthData.m_growthDuration);
					m_player->SetMaxSpeed(m_player->GetMaxSpeed() / m_slowAmount);
					m_growthData.m_growing = true;
				}

				if (m_player->GetAnimController()->FinishedScale())
				{
					m_shouldDestroy = true;
				}
				break;

			case ItemGrowthData::MAINTIAIN:
			{
				if (!m_slowed)
				{
					m_player->setVelocity(m_player->getVelocity() * m_slowAmount);
					m_player->SetMaxSpeed(m_player->GetMaxSpeed() * m_maxSpeedMutli);
					m_slowed = true;
				}

				m_growthData.m_sizeChangeTimeElapsed += Locator::getGSD()->m_dt;
				if (m_growthData.m_sizeChangeTimeElapsed >= m_growthData.m_sizeChangeDuration)
				{
					m_growthData.m_scaleState = ItemGrowthData::GROW;
				}

				break;
			}
			case ItemGrowthData::SHRINK:
				if (!m_growthData.m_shrinking)
				{
					m_player->Scale(m_growthData.m_shrinkScale, m_growthData.m_shrinkDuration);
					m_growthData.m_shrinking = true;
					Locator::getAudio()->Play(SoundType::ITEMS, static_cast<int>(ItemSounds::LIGHTNING_SOUND));
				}
				m_player->Spin(m_playerSpinRev, m_playerSpinDuration);

				if (m_player->GetAnimController()->FinishedScale())
				{
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
				m_growthData.m_growScale = m_player->GetScale();
				m_growthData.m_shrinkScale = m_growthData.m_growScale * m_growthData.m_scaleMulti;
				m_growthData.m_sizeChangeDuration -= m_player->GetRanking();
				m_itemMesh->m_displayedMesh->SetShouldRender(false);
			}
		}
	}
}

void LightningCloud::Use(Player * player, bool _altUse)
{
	if (player->isInvincible())
	{
		m_shouldDestroy = true;
		return;
	}

	setItemInUse(player);
	m_trailingPlayerImmunity = true;
}