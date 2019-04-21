#include "pch.h"
#include "MoveAI.h"
#include <stack>
#include <iostream>

MoveAI::MoveAI(PhysModel* _model, ControlledMovement* _move) : m_model(_model), m_move(_move)
{
	for (int i = 0; i < m_maxPathIterations; i++)
	{
		m_debugCups.push_back(new SDKMeshGO3D("DEFAULT_ITEM"));
		m_debugCups.at(i)->Load();
	}
}

void MoveAI::DebugRender()
{
	
	for (SDKMeshGO3D* mesh : m_debugCups)
	{
		mesh->Render();
	}
	
}

void MoveAI::Update(Track* _track)
{
	_track->SetValidCollision(true, false, true, false);
	m_route.clear();
	Matrix world = m_model->GetWorld();
	Vector3 pos = m_model->GetPos();
	Vector3 direction = world.Forward();
	int iterations = 0;

	DebugText::print("AI DEBUG, CURRENT WAYPOINT: " + std::to_string(m_move->GetWaypoint()));

	if (m_move->GetWaypoint() == _track->getWaypoints().size() - 1)
	{
		m_waypointPos = _track->getWaypoints()[0];
	}
	else
	{
		m_waypointPos = _track->getWaypoints()[m_move->GetWaypoint()+1];
	}

	if (FindRoute(_track, world, pos, direction, iterations, 1))
	{
		for (int i = 0; i < m_route.size(); i++)
		{
			m_debugCups[i]->SetPos(m_route[i]);
			m_debugCups[i]->UpdateWorld();
		}
	}
	else
	{
		m_route.clear();
		Vector3 waypointDir = m_waypointPos - pos;
		waypointDir.Normalize();
		FindWorld(_track, world, world, pos, pos, waypointDir, 1);
		m_route.push_back(pos);
	}

	/*
	Vector3 left = m_model->GetWorld().Forward() + m_model->GetWorld().Left();
	left.Normalize();
	Vector3 right = m_model->GetWorld().Forward() + m_model->GetWorld().Right();
	right.Normalize();

	int forwardCount = GetStepsForDirection(_track, m_model->GetWorld().Forward() * m_aiPathStep);
	int leftCount = GetStepsForDirection(_track, left * m_aiPathStep);
	int rightCount = GetStepsForDirection(_track, right * m_aiPathStep);

	DebugText::print("AI DEBUG, FORWARD: " + std::to_string(forwardCount) + ", LEFT: " + std::to_string(leftCount) + ", RIGHT: " + std::to_string(rightCount));

	// If off the track and every direction is 0, check hard left/right to get back on the track
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
	else if (leftCount >= rightCount)
	{
		m_move->TurnLeft(true);
	}
	else if(rightCount > leftCount)
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
	*/
}

bool MoveAI::FindRoute(Track* _track, Matrix& _world, Vector3& _pos, Vector3& _direction, int& _iterations, int _localIterations)
{
	// Get start pos

	// int iterations
	// int localiter
	// While steps to go
	//	 If forward dist too short
	//     if localiter > 0
	//      if FindRoute(right)
	//         return true
	//      else return FindRoute(left)
	//   else if left dist too short
	//       push if forward+left valid
	//   else if right dist too short
	//       push if forward+left valid
	//   else forward

	while (_iterations < m_maxPathIterations)
	{
		Matrix newWorld = Matrix::Identity;
		Vector3 newPos = Vector::Zero;
		
		int count = FindWorld(_track, _world, newWorld, _pos, newPos, _direction, m_minFrontSpace);
		if (count < m_minFrontSpace)
		{
			if (_localIterations > 0)
			{
				Vector right;
				Vector left;
				if (count < m_minFrontSpace / 2)
				{
					right = _world.Right();
					left = _world.Left();
				}
				else
				{
					right = _world.Right() + _world.Forward();
					left = _world.Left() + _world.Forward();
					right.Normalize();
					left.Normalize();
				}
				if (FindRoute(_track, _world, _pos, right, _iterations, 0))
				{
					return true;
				}
				else return(FindRoute(_track, _world, _pos, left, _iterations, 0));
			}
			else
			{
				return false;
			}
		}
		else
		{
			FindWorld(_track, _world, newWorld, _pos, newPos, _direction, 1);
			_world = newWorld;
			_pos = newPos;
			_direction = _world.Forward();
			_iterations++;
			_localIterations++;
			m_route.push_back(_pos);
		}
	}
	return true;
}

int MoveAI::FindWorld(Track* _track, const Matrix& _startWorld, Matrix& _endWorld, const Vector3& _startPos, Vector3& _endPos, Vector3 _direction, const int& _steps)
{
	_direction *= m_aiPathStep;

	// Check to see if this direction diverges from the waypoint
	if (Vector3::DistanceSquared(_startPos, m_waypointPos) < Vector3::DistanceSquared(_startPos + _direction, m_waypointPos))
	{
		return 0;
	}

	Vector3 intersect = Vector3::Zero;
	MeshTri* tri = nullptr;

	_endWorld = _startWorld;
	_endPos = _startPos;

	for (int i = 0; i < _steps; i++)
	{
		if (_track->DoesLineIntersect(_endWorld.Down()*(m_model->data.m_height * 10), _endPos + (_endWorld.Up() * (m_model->data.m_height * 2)), intersect, tri, 0.4f))
		{
			Vector secondIntersect;
			MeshTri* tri2 = nullptr;
			tri->DoesLineIntersect(_endWorld.Down() * (m_model->data.m_height * 10), _endPos + _direction + (_endWorld.Up() * (m_model->data.m_height * 2)), secondIntersect, tri2, 0.4f);

			_endWorld = Matrix::CreateWorld(secondIntersect, secondIntersect - intersect, tri->m_plane.Normal());
			_direction = _endWorld.Forward() * m_aiPathStep;

			Vector scale = Vector::Zero;
			Quaternion rot = Quaternion::Identity;
			_endWorld.Decompose(scale, rot, _endPos);
		}
		else
		{
			return i;
		}
	}
	return _steps;
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
			// If off the track on the first iteration set a flag
			if (iterations == 0)
			{
				m_offTrack = true;
			}
			else if(!m_offTrack)
			{
				return iterations;
			}
		}
		// If off the track we instead count the distance to valid track
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