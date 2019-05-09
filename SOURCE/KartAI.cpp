#include "pch.h"
#include "KartAI.h"
#include "Player.h"
#include "RaceManager.h"

KartAI::KartAI(PhysModel* _model, ControlledMovement* _move) : MoveAI(_model, _move)
{
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	m_generator = std::mt19937(seed);
	m_probabilityRoll = std::uniform_int_distribution<int>(m_minItemDelay, m_maxItemDelay);
}


KartAI::~KartAI()
{
}


bool KartAI::Update()
{
	MoveAI::Update();

	//if (!Locator::getRM()->attract_state)
	{
		ItemType type = m_player->GetItemInInventory();

		if (m_player->IsTrailingItem())
		{
			m_player->TrailItems();
		}

		if (!m_hasItem && type != NONE || m_player->IsTrailingItem())
		{
			m_hasItem = true;
			m_timeForNextItem = m_probabilityRoll(m_generator);
		}

		if (m_hasItem)
		{
			m_timeElapsed += Locator::getGSD()->m_dt;

			if (m_timeElapsed > m_timeForNextItem && ShouldSpeedBoost(type))
			{
				if (m_player->IsTrailingItem())
				{
					m_player->ReleaseItem();
					m_timeElapsed = 0;
					m_hasItem = false;
				}
				else
				{
					m_player->SpawnItems(type);
					m_timeElapsed = 0;
					m_hasItem = false;
				}
			}
		}
	}

	return false;
}

bool KartAI::ShouldSpeedBoost(const ItemType& type)
{
	if (type == MUSHROOM || type == MUSHROOM_3X || type == MUSHROOM_UNLIMITED)
	{
		if(m_routeIndex >= m_route.size())
			return Vector3::Distance(m_route[m_routeIndex].position, m_player->GetPos()) < m_minDistForBoost;
	}
	return true;
}