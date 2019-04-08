#include "pch.h"
#include "TripleMushroom.h"
#include "GarbageCollector.h"


TripleMushroom::TripleMushroom()
{
	for (int i = 0; i < 3; i++)
	{
		Mushroom* mushroom = new Mushroom();
		m_mushrooms.push(mushroom);
	}
}


TripleMushroom::~TripleMushroom()
{
	for (int i = 0; i < m_mushrooms.size(); i++)
	{
		Locator::getGarbageCollector()->DeletePointer(m_mushrooms.front());
		m_mushrooms.pop();
	}
}

void TripleMushroom::Tick()
{
	if (m_currentMushroom)
	{
		m_currentMushroom->Tick();
		
		if (m_currentMushroom->ShouldDestroy())
		{
			Locator::getGarbageCollector()->DeletePointer(m_currentMushroom);

			if (m_mushrooms.empty())
			{
				FlagForDestoy();
			}
		}
	}
}

void TripleMushroom::HitByPlayer(Player * player)
{
}

void TripleMushroom::Use(Player * player, bool _altUse)
{
	m_mushrooms.front()->Use(player, false);
	m_currentMushroom = m_mushrooms.front();
	m_mushrooms.pop();
}
