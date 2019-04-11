#include "pch.h"
#include "AIScheduler.h"


AIScheduler::AIScheduler(Track* _track) : m_track(_track)
{
}


AIScheduler::~AIScheduler()
{
}

void AIScheduler::Update()
{
	for (MoveAI* ai : m_aiList)
	{
		ai->Update(m_track);
	}
}

void AIScheduler::AddAI(MoveAI* _ai)
{
	m_aiList.push_back(_ai);
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
}


