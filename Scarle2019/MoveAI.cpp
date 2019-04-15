#include "MoveAI.h"
#include "pch.h"
#include <stack>
#include <iostream>

MoveAI::MoveAI(PhysModel* _model, ControlledMovement* _move) : m_model(_model), m_move(_move)
{
}

void MoveAI::Update(Track* _track)
{
	_track->SetValidCollision(true, true, true, false);
	Vector3 left = m_model->GetWorld().Forward() + m_model->GetWorld().Left();
	left.Normalize();
	Vector3 right = m_model->GetWorld().Forward() + m_model->GetWorld().Right();
	right.Normalize();

	int forwardCount = GetStepsForDirection(_track, m_model->GetWorld().Forward() * m_aiPathStep);
	int leftCount = GetStepsForDirection(_track, left * m_aiPathStep);
	int rightCount = GetStepsForDirection(_track, right * m_aiPathStep);

	std::cout << forwardCount << ", " << leftCount << ", " << rightCount << std::endl;

	// If off the track and no every direction is 0, check hard left/right to get back on the track
	if (m_offTrack && !forwardCount && !leftCount && !rightCount)
	{
		if (GetStepsForDirection(_track, m_model->GetWorld().Left() * m_aiPathStep))
		{
			m_move->TurnLeft(true);
		}
		else if (GetStepsForDirection(_track, m_model->GetWorld().Right() * m_aiPathStep))
		{
			m_move->TurnRight(true);
		}
	}
	else if (forwardCount >= rightCount && forwardCount >= leftCount)
	{
		m_move->TurnLeft(false);
		m_move->TurnRight(false);
	}
	else if (leftCount >= rightCount || rightCount < m_aiPathStep/10)
	{
		m_move->TurnLeft(true);
	}
	else if(rightCount > leftCount || leftCount < m_aiPathStep / 10)
	{
		m_move->TurnRight(true);
	}

	if (forwardCount < m_maxPathIterations * 0.7f)
	{
		m_move->Drift(true);
		m_move->setAcceleration(1);
	}
	else if (forwardCount < m_maxPathIterations * 0.4f)
	{
		m_move->Drift(true);
		m_move->setAcceleration(0);
	}
	else if (forwardCount < m_maxPathIterations * 0.2f)
	{
		m_move->Drift(true);
		m_move->setAcceleration(-1);
	}
	else
	{
		m_move->Drift(false);
		m_move->setAcceleration(1);
	}
}

int MoveAI::GetStepsForDirection(Track* _track, Vector3 _direction)
{
	Matrix world = m_model->GetWorld();
	Vector3 pos = m_model->GetPos();
	Vector3 intersect = Vector3::Zero;
	float length = _direction.Length();

	MeshTri* tri = nullptr;

	int iterations = 0;
	m_offTrack = false;

	while (iterations < m_maxPathIterations && _track->DoesLineIntersect(world.Down()*(m_model->data.m_height * 10), pos + (world.Up() * (m_model->data.m_height * 2)), intersect, tri, 0.4f))
	{
		if (tri->GetType() == CollisionType::OFF_TRACK)
		{
			// If the kart is off the track on the first iteration set a flag
			if (iterations == 0)
			{
				m_offTrack = true;
			}
			else if(!m_offTrack)
			{
				return iterations;
			}
		}
		// If the kart started off the track we instead count the distance to valid track
		if (m_offTrack)
		{
			// So we return once we hit it
			if (tri->GetType() != CollisionType::OFF_TRACK)
			{
				return m_maxPathIterations - iterations;
			}
		}
		Vector secondIntersect;
		MeshTri* tri2 = nullptr;
		tri->DoesLineIntersect(world.Down() * (m_model->data.m_height * 10), pos + _direction + (world.Up() * (m_model->data.m_height *2)), secondIntersect, tri2, 0.4f);

		world = Matrix::CreateWorld(secondIntersect, secondIntersect - intersect, tri->m_plane.Normal());
		_direction = world.Forward() * length;

		Vector scale = Vector::Zero;
		Quaternion rot = Quaternion::Identity;
		world.Decompose(scale, rot, pos);

		iterations++;
	}

	if (m_offTrack)
	{
		return 0;
	}
	return iterations;
}