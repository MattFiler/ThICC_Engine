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
	return m_nextState;
}

void CharecterSelectionState::Render()
{

}

bool CharecterSelectionState::Load()
{

}

void CharecterSelectionState::Cleanup()
{

}
