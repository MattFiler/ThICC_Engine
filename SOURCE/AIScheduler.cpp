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

void AIScheduler::Update()
{
	m_elapsedTime += Locator::getGSD()->m_dt;

	for (MoveAI* ai : m_aiList)
	{
		ai->Update();
	}

	for (size_t i = 0; i < m_aiList.size(); i++)
	{
		if (i == m_currentIndex && m_elapsedTime >= (m_totalFrequency*i))
		{
			m_aiList[i]->RecalculateLine(m_track);
			m_currentIndex++;
		}
	}

	if (m_elapsedTime > m_lineUpdateFrequency)
	{
		m_elapsedTime -= m_lineUpdateFrequency;
		m_currentIndex = 0;
	}
}

void AIScheduler::AddAI(MoveAI* _ai)
{
	m_aiList.push_back(_ai);
	m_totalFrequency = m_lineUpdateFrequency / m_aiList.size();
}

void AIScheduler::RemoveAI(MoveAI* _ai)
{
	for (int i = 0; i < m_aiList.size(); i++)
	{
		if (m_aiList[i] == _ai)
		{
			m_aiList.erase(m_aiList.begin() + i);
			return;
		}
	}
	m_totalFrequency = m_lineUpdateFrequency / m_aiList.size();
}


