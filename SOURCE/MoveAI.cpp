#include "pch.h"
#include "MoveAI.h"
#include <stack>
#include <iostream>

MoveAI::MoveAI(PhysModel* _model, ControlledMovement* _move) : m_model(_model), m_move(_move)
{
}

void MoveAI::Update(Track* _track)
{
	Vector3 left = m_model->GetWorld().Forward() + m_model->GetWorld().Left();
	left.Normalize();
	Vector3 right = m_model->GetWorld().Forward() + m_model->GetWorld().Right();
	right.Normalize();

	int forwardCount = GetStepsForDirection(_track, m_model->GetWorld().Forward() * m_aiPathStep);
	int leftCount = GetStepsForDirection(_track, left * m_aiPathStep);
	int rightCount = GetStepsForDirection(_track, right * m_aiPathStep);

	std::cout << forwardCount << ", " << leftCount << ", " << rightCount << std::endl;

	if (forwardCount >= rightCount && forwardCount >= leftCount)
	{
		m_move->TurnLeft(false);
		m_move->TurnRight(false);
	}
	else if (leftCount > rightCount)
	{
		m_move->TurnLeft(true);
	}
	else
	{
		m_move->TurnRight(true);
	}

	m_move->setAcceleration(1);
}

int MoveAI::GetStepsForDirection(Track* _track, Vector3 _direction)
{
	Matrix world = m_model->GetWorld();
	Vector3 pos = m_model->GetPos();
	Vector3 intersect = Vector3::Zero;

	MeshTri* tri = nullptr;

	int iterations = 0;

	while (iterations < m_maxPathIterations && _track->DoesLineIntersect(world.Down()*(m_model->data.m_height * 10), pos + (world.Up() * (m_model->data.m_height * 2)), intersect, tri, 0.4f))
	{
		Vector secondIntersect;
		MeshTri* tri2 = nullptr;
		tri->DoesLineIntersect(world.Down() * (m_model->data.m_height * 10), pos + _direction + (world.Up() * (m_model->data.m_height * 2)), secondIntersect, tri2, 0.4f);

		world = Matrix::CreateWorld(pos + _direction, secondIntersect - intersect, tri->m_plane.Normal());
		_direction = world.Forward();

		Vector scale = Vector::Zero;
		Quaternion rot = Quaternion::Identity;
		world.Decompose(scale, rot, pos);

		iterations++;
	}

	return iterations;
}