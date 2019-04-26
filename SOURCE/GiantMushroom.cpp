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
	m_growthData.m_growthSpeed = (float)m_itemData["MUSHROOM_GIANT"]["info"]["grow"]["growth_speed"];
	m_growthData.m_shrinkSpeed = (float)m_itemData["MUSHROOM_GIANT"]["info"]["grow"]["shrink_speed"];
	m_growthData.m_duration = (float)m_itemData["MUSHROOM_GIANT"]["info"]["grow"]["growth_duration"];
}

void GiantMushroom::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		switch (m_growthData.m_scaleState)
		{
		case ItemGrowthData::GROW:
			m_player->SetScale(Vector3::Lerp(m_growthData.m_startScale, m_growthData.m_endScale, m_growthData.m_scalePercent));
			m_player->UpdateWorld();

			DebugText::print("x: " + std::to_string(m_player->GetScale().x) + " y: "  + std::to_string(m_player->GetScale().y) + " z: " + std::to_string(m_player->GetScale().z));

			m_growthData.m_scalePercent += m_growthData.m_growthSpeed * Locator::getGSD()->m_dt;

			if (m_growthData.m_scalePercent >= 1)
			{
				m_growthData.m_scalePercent = 1;
				m_growthData.m_scaleState = ItemGrowthData::MAINTIAIN;
			}
			break;

		case ItemGrowthData::MAINTIAIN:

			m_player->setInvicible(m_growthData.m_timeElapsed < m_growthData.m_duration);
			m_growthData.m_timeElapsed += Locator::getGSD()->m_dt;
			if (m_growthData.m_timeElapsed >= m_growthData.m_duration)
			{
				m_growthData.m_scaleState = ItemGrowthData::SHRINK;
			}
			break;

		case ItemGrowthData::SHRINK:
			m_player->setInvicible(false);
			m_player->SetScale(Vector3::Lerp(m_growthData.m_startScale, m_growthData.m_endScale, m_growthData.m_scalePercent));
			m_player->UpdateWorld();

			m_growthData.m_scalePercent -= m_growthData.m_shrinkSpeed * Locator::getGSD()->m_dt;

			if (m_growthData.m_scalePercent <= 0)
			{
				m_growthData.m_scalePercent = 0;
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
		m_growthData.m_startScale = m_player->GetScale();
		m_growthData.m_endScale = m_growthData.m_startScale * m_growthData.m_scaleMulti;
	}
}

