#pragma once
#include "State.h"

class TrackSelectionState : public State
{
public:
	TrackSelectionState();
	~TrackSelectionState();

	States Update(GameStateData* _GSD, InputData* _ID) override;
	void Render() override;
	bool Load() override;
	void Cleanup() override;

private:

};

