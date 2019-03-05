#include "pch.h"
#include "StateManager.h"
#include "CharecterSelectionState.h"
#include "TrackSelectionState.h"


StateManager::StateManager()
{
}


StateManager::~StateManager()
{
}

int StateManager::Update(GameStateData * _GSD, InputData * _ID)
{
	if (m_currState != nullptr)
	{
		switch (m_currState->Update(_GSD, _ID))
		{
		case States::NONE:
			return 0;
		case States::CHARECTER_SELECTION:
			ChangeState(new CharecterSelectionState);
			break;
		case States::TRACK_SELECTION:
			ChangeState(new TrackSelectionState);
			break;
		default:
			return -1;
			break;
		}
	}
	return 1;
}

void StateManager::Render(RenderData * _RD)
{
}

bool StateManager::Load(GameStateData * _GSD, RenderData * _RD, InputData * _ID, WindowData * _WD)
{
	return false;
}


void StateManager::ChangeState(State* _newState)
{
	delete m_currState;
	m_currState = _newState;
}

