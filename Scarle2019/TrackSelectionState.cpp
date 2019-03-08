#include "pch.h"
#include "TrackSelectionState.h"

TrackSelectionState::TrackSelectionState()
{

}

TrackSelectionState::~TrackSelectionState()
{

}

States TrackSelectionState::Update(GameStateData * _GSD, InputData * _ID)
{
	return m_nextState;
}

void TrackSelectionState::Render()
{
}

bool TrackSelectionState::Load()
{
	return false;
}

void TrackSelectionState::Cleanup()
{
}
