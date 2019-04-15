#pragma once
#include "PhysModel.h"
#include "Track.h"
#include "ControlledMovement.h"

class MoveAI
{
public:
	MoveAI(PhysModel* _model, ControlledMovement* _move);

	void Update(Track* _track);

	void UseDrift(bool _flag) { m_useDrift = _flag;};
private:
	
	int GetStepsForDirection(Track* _track, Vector3 _direction);

	PhysModel* m_model = nullptr;
	ControlledMovement* m_move = nullptr;

	float m_aiPathStep = 5;
	int m_maxPathIterations = 15;
	bool m_offTrack = false;

	bool m_useDrift = false;
};