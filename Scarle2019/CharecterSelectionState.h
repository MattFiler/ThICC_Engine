#pragma once
#include "State.h"

class CharecterSelectionState : public State	
{
public:
	CharecterSelectionState();
	~CharecterSelectionState();

	 States Update(GameStateData* _GSD, InputData* _ID) override;
	 void Render() override;
	 bool Load() override;
	 void Cleanup() override;


};

