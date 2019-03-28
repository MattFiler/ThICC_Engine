#pragma once
#include "StepTimer.h"
#include "Constants.h"
#include "InputData.h"
#include "WindowData.h"

class State
{
public:
	State() = default;
	~State() {};

	virtual States Update(GameStateData* _GSD, InputData* _ID)				= 0;
	virtual void Render()													= 0;
	virtual bool Load()														= 0;
	virtual void Cleanup()													= 0;

protected:
	//States m_nextState = States::NOSTATE;
};

