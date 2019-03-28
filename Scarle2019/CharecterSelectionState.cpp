#include "pch.h"
#include "CharecterSelectionState.h"


CharecterSelectionState::CharecterSelectionState()
{

}


CharecterSelectionState::~CharecterSelectionState()
{

}

States CharecterSelectionState::Update(GameStateData * _GSD, InputData * _ID)
{
	return States::END;
}

void CharecterSelectionState::Render()
{

}

bool CharecterSelectionState::Load()
{
	return true;
}

void CharecterSelectionState::Cleanup()
{

}
