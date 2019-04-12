#pragma once
class AIScheduler
{
public:
	AIScheduler(Track* _track);
	~AIScheduler();

	void Update();
	void AddAI(MoveAI* _ai);
	void RemoveAI(MoveAI* _ai);
private:

	std::vector<MoveAI*> m_aiList;
	Track* m_track = nullptr;
};

