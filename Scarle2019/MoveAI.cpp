#include "MoveAI.h"
#include "pch.h"
#include <stack>

MoveAI::MoveAI(PhysModel* _model, ControlledMovement* _move) : m_model(_model), m_move(_move)
{
}

void MoveAI::Update(Track* _track)
{
	Matrix world = m_model->GetWorld();
	Vector3 pos = m_model->GetPos();

	Vector3 intersect = Vector3::Zero;
	MeshTri* tri = nullptr;

	std::stack<Matrix> forwardWorlds;
	std::stack<Vector3> forwardPos;

	int iterations = 0;
	bool turning = false;

	while (iterations < m_maxPathIterations && _track->DoesLineIntersect(world.Down()*(m_model->data.m_height * 10), pos + (world.Up() * (m_model->data.m_height / 2)), intersect, tri, 0.4f))
	{
		Vector secondIntersect;
		MeshTri* tri2 = nullptr;
		tri->DoesLineIntersect(world.Down() * (m_model->data.m_height * 10), pos + m_model->getVelocity() + world.Forward() + (world.Up() * (m_model->data.m_height / 2)), secondIntersect, tri2, 0.4f);

		world = Matrix::CreateWorld(pos + m_model->getVelocity() * m_aiPathStep, secondIntersect - intersect, tri->m_plane.Normal());

		Vector scale = Vector::Zero;
		Quaternion rot = Quaternion::Identity;
		world.Decompose(scale, rot, pos);

		forwardWorlds.push(world);
		forwardPos.push(pos);

		iterations++;
	}


	// If no track was found on one of the iterations
	if (iterations != m_maxPathIterations)
	{
		// Unwind the stack checking for a valid direction to steer that would be on track
		while (!forwardWorlds.empty())
		{
			Vector3 vel = m_model->getVelocity() * m_aiPathStep;
			Vector3 velLeft = vel;
			velLeft.Normalize();
			Vector3 velRight = velLeft;
			velLeft += forwardWorlds.top().Left();
			velLeft.Normalize();
			velLeft *= vel.Length();
			velRight += forwardWorlds.top().Right();
			velRight *= vel.Length();

			if (_track->DoesLineIntersect(forwardWorlds.top().Down()*(m_model->data.m_height * 10), forwardPos.top() + velLeft + (forwardWorlds.top().Up() * (m_model->data.m_height / 2)), intersect, tri, 0.4f))
			{
				m_move->TurnLeft(true);
				turning = true;
			}
			else if (_track->DoesLineIntersect(forwardWorlds.top().Down()*(m_model->data.m_height * 10), forwardPos.top() + velRight + (forwardWorlds.top().Up() * (m_model->data.m_height / 2)), intersect, tri, 0.4f))
			{
				m_move->TurnRight(true);
				turning = true;
			}

			forwardWorlds.pop();
			forwardPos.pop();
		}
	}

	if (!turning)
	{
		m_move->TurnLeft(false);
		m_move->TurnRight(false);
	}
	m_move->setAcceleration(1);
}