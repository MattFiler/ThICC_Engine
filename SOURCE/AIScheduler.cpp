#include "pch.h"
#include "AIScheduler.h"
#include "GameStateData.h"

AIScheduler::AIScheduler()
{
}


AIScheduler::~AIScheduler()
{
}

void AIScheduler::UpdateTrack(Track* _track) {
	m_track = _track;
}

void AIScheduler::DebugRender()
{ 
#ifdef _DEBUG
	for (MoveAI* ai : m_aiList)
	{
		ai->DebugRender();
	}
#endif
}

void AIScheduler::Update()
{
	if (!enabled) {
		return;
	}

	#ifndef _ARCADE
	m_elapsedTime += Locator::getGSD()->m_dt;

	for (size_t i = 0; i < m_aiList.size(); i++)
	{
		if (i == m_currentIndex)
		{
			if (m_elapsedTime >= (m_totalFrequency*i) && m_aiList[i])
			{
				m_aiList[i]->RecalculateLine(m_track);
				m_currentIndex++;
			}
			else
			{
				break;
			}
		}
	}

	if (m_elapsedTime > m_lineUpdateFrequency)
	{
		m_elapsedTime -= m_lineUpdateFrequency;
		m_currentIndex = 0;
	}

	for (size_t i = 0; i < m_aiList.size(); i++)
	{
		if (m_aiList[i]->Update())
		{
			m_aiList[i]->RecalculateLine(m_track);
		}
	}
	#endif
}

void AIScheduler::AddAI(MoveAI* _ai)
{
	#ifndef _ARCADE
	m_aiList.push_back(_ai);
	m_totalFrequency = m_lineUpdateFrequency / m_aiList.size();
	#endif
}

void AIScheduler::RemoveAI(MoveAI* _ai)
{
	#ifndef _ARCADE
	for (int i = 0; i < m_aiList.size(); i++)
	{
		if (m_aiList[i] == _ai)
		{
			m_aiList.erase(m_aiList.begin() + i);
			return;
		}
	}
	m_totalFrequency = m_lineUpdateFrequency / m_aiList.size();
	#endif
}


