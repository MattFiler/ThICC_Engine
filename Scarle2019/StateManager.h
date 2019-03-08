#pragma once
#include "State.h"
#include "StepTimer.h"
#include "Constants.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	int Update(GameStateData* _GSD, InputData* _ID);

	void Render(RenderData* _RD);
	bool Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD);

	State* m_currState = nullptr;

private:
	void ChangeState(State* _newScene);
};

