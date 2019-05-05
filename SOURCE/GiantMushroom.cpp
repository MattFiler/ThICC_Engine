#include "pch.h"
#include "GiantMushroom.h"
#include "Player.h"
#include "GameStateData.h"
#include "DebugText.h"

GiantMushroom::GiantMushroom()
{
	InitMushroomData();
	m_shouldDespawn = false;
}

void GiantMushroom::InitMushroomData()
{
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	m_itemData << i;
	m_growthData.m_scaleMulti = (float)m_itemData["MUSHROOM_GIANT"]["info"]["grow"]["size_multiplier"];
	m_growthData.m_growthDuration = (float)m_itemData["MUSHROOM_GIANT"]["info"]["grow"]["growth_duration"];
	m_growthData.m_shrinkDuration = (float)m_itemData["MUSHROOM_GIANT"]["info"]["grow"]["shrink_duraion"];
	m_growthData.m_sizeChangeDuration = (float)m_itemData["MUSHROOM_GIANT"]["info"]["grow"]["max_growth_duration"];
}

void GiantMushroom::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		switch (m_growthData.m_scaleState)
		{
		case ItemGrowthData::GROW:
			if (!m_growthData.m_growing)
			{
				m_player->Scale(m_growthData.m_growScale, m_growthData.m_growthDuration);
				m_growthData.m_growing = true;

			}

			if (m_player->GetAnimController()->FinishedScale())
			{
				m_player->setInvicible(true);
				m_growthData.m_scaleState = ItemGrowthData::MAINTIAIN;
			}
			break;

		case ItemGrowthData::MAINTIAIN:
			m_player->setInvicible(m_growthData.m_sizeChangeTimeElapsed < m_growthData.m_sizeChangeDuration);
			m_growthData.m_sizeChangeTimeElapsed += Locator::getGSD()->m_dt;
			if (m_growthData.m_sizeChangeTimeElapsed >= m_growthData.m_sizeChangeDuration)
			{
				m_growthData.m_scaleState = ItemGrowthData::SHRINK;
			}
			break;

		case ItemGrowthData::SHRINK:
			if (!m_growthData.m_shrinking)
			{
				m_player->Scale(m_growthData.m_shrinkScale, m_growthData.m_shrinkDuration);
				m_player->setInvicible(false);
				m_growthData.m_shrinking = true;
			}

			if (m_player->GetAnimController()->FinishedScale())
			{
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
		m_growthData.m_shrinkScale = m_player->GetScale();
		m_growthData.m_growScale = m_growthData.m_shrinkScale * m_growthData.m_scaleMulti;
	}
}

