#pragma once
#include "PhysModel.h"
#include "Track.h"
#include "ControlledMovement.h"

class MoveAI
{
public:
	MoveAI(PhysModel* _model, ControlledMovement* _move);

	void Update(Track* _track);

private:
	
	PhysModel* m_model = nullptr;
	ControlledMovement* m_move = nullptr;

	float m_aiPathStep = 0.2f;
	int m_maxPathIterations = 6;
};