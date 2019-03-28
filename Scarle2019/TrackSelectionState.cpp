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
	//set to no state, needs changing had error.
	return States::NOSTATE;
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
