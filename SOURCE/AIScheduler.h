#pragma once
#include "pch.h"
#include "Track.h"
#include "MoveAI.h"

class AIScheduler
{
public:
	AIScheduler();
	~AIScheduler();

	void UpdateTrack(Track* _track);
	void DebugRender();

	void Update();
	void AddAI(MoveAI* _ai);
	void RemoveAI(MoveAI* _ai);
private:

	float m_lineUpdateFrequency = 1;

	float m_totalFrequency = 0.1f;
	float m_elapsedTime = 0;
	size_t m_currentIndex = 0;
	std::vector<MoveAI*> m_aiList;
	Track* m_track = nullptr;
};

