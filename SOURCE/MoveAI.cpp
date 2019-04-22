#include "pch.h"
#include "MoveAI.h"
#include <stack>
#include <iostream>

MoveAI::MoveAI(PhysModel* _model, ControlledMovement* _move) : m_model(_model), m_move(_move)
{
	for (int i = 0; i < m_maxPathIterations; i++)
	{
		m_debugRaceLine.push_back(new SDKMeshGO3D("DEFAULT_ITEM"));
		m_debugRaceLine.back()->Load();
		Vector3 new_scale = m_debugRaceLine.back()->GetScale() * ((float)(i+1) / m_maxPathIterations);
		m_debugRaceLine.back()->SetScale(new_scale);
		m_debugRaceLine.back()->UpdateWorld();
	}
	for (int i = 0; i < 20; i++)
	{
		m_debugNextWaypoint.push_back(new SDKMeshGO3D("DEFAULT_ITEM"));
		m_debugNextWaypoint.back()->Load();
		m_debugNextWaypoint.back()->SetScale(0.2f);
		m_debugNextWaypoint.back()->UpdateWorld();
	}
}

void MoveAI::DebugRender()
{
	
	for (SDKMeshGO3D* mesh : m_debugRaceLine)
	{
		mesh->Render();
	}
	for (SDKMeshGO3D* mesh : m_debugNextWaypoint)
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

	/* 
	For now I'm grabbing the top_left position from the waypoint's plane mesh. This should ideally be updated to utilise all four corner points.

	if (m_move->GetWaypoint() == (int)(_track->getWaypoints().size() - 1))
	{
		m_waypointPos = 0;
	}
	else
	{
		m_waypointPos = m_move->GetWaypoint() + 1;
	}	*/
	m_waypointPos = m_move->GetWaypoint();


	Vector3 diff = _track->getWaypointMiddle(m_waypointPos) - pos;
	diff = diff * (1.0f / m_debugNextWaypoint.size());
	for (int i = 0; i < m_debugNextWaypoint.size(); i++)
	{
		m_debugNextWaypoint[i]->SetPos(pos + (i*diff));
		m_debugNextWaypoint[i]->UpdateWorld();
	}

	if (FindRoute(_track, world, pos, direction, iterations, true, m_waypointPos))
	{
		for (int i = 0; i < m_route.size(); i++)
		{
			m_debugRaceLine[i]->SetPos(m_route[i]);
			m_debugRaceLine[i]->UpdateWorld();
		}
	}
	else
	{
		m_route.clear();
		Vector3 waypointDir = _track->getWaypoints()[m_waypointPos].middle_bottom - pos;
		waypointDir.Normalize();
		FindWorld(_track, world, world, pos, pos, waypointDir, 1, 0, m_waypointPos);
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

bool MoveAI::FindRoute(Track* _track, Matrix& _world, Vector3& _pos, Vector3& _direction, int _iterations, bool _allowTurn, int _waypointIndex)
{
	while (_iterations < m_maxPathIterations)
	{
		Matrix newWorld = Matrix::Identity;
		Vector3 newPos = Vector::Zero;
		
		// Check to see if straight ahead is valid
		int count = FindWorld(_track, _world, newWorld, _pos, newPos, _direction, m_minFrontSpace, _iterations, _waypointIndex);
		// If straight ahead isn't valid
		if (count < m_minFrontSpace)
		{
			// Try left and right recurring this method to find a valid route
			if (_allowTurn)
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
				if (FindRoute(_track, _world, _pos, right, _iterations, false, _waypointIndex))
				{
					return true;
				}
				else
				{
					m_route.resize(_iterations);
				}

				return(FindRoute(_track, _world, _pos, left, _iterations, false, _waypointIndex));
			}
			// Return straight away if this is the first iteration, this prevents going down a pointless tree
			else
			{
				return false;
			}
		}
		// If straight ahead is valid
		else
		{
			// Find the world forward of this one
			FindWorld(_track, _world, newWorld, _pos, newPos, _direction, 1, _iterations, _waypointIndex);
			_world = newWorld;
			_pos = newPos;
			_direction = _world.Forward();
			_iterations++;
			_allowTurn = true;
			m_route.push_back(_pos);
		}
	}
	return true;
}

int MoveAI::FindWorld(Track* _track, const Matrix& _startWorld, Matrix& _endWorld, const Vector3& _startPos, Vector3& _endPos, Vector3 _direction, const int& _steps, const int& _iteration, int &_waypointIndex)
{
	_direction *= m_aiPathStep;

	// Check to see if this direction diverges from the waypoint
	float diff = Vector3::DistanceSquared(_startPos, _track->getWaypointMiddle(_waypointIndex)) - Vector3::DistanceSquared(_startPos + _direction, _track->getWaypointMiddle(_waypointIndex));
	// If diff is negative they are diverging. So if they are diverging by more than half the step
	if (_iteration > 0 && diff < 0)
	{
		// Check to see if we are also divering from the next waypoint
		if (Vector3::DistanceSquared(_startPos, _track->getWaypointMiddle(_waypointIndex+1)) > Vector3::DistanceSquared(_startPos + _direction, _track->getWaypointMiddle(_waypointIndex+1)))
		{
			// If not, use this waypoint instead of the current one
			_waypointIndex++;
		}
		else
		{
			// Otherwise this direction is in-valid
			return 0;
		}
	}

	Vector3 intersect = Vector3::Zero;
	MeshTri* tri = nullptr;

	_endWorld = _startWorld;
	_endPos = _startPos;

	for (int i = 0; i < _steps; i++)
	{
		if (_track->DoesLineIntersect(_endWorld.Down()*(m_model->data.m_height * 10), _endPos + (_endWorld.Up() * (m_model->data.m_height * 2)), intersect, tri, 0.4f))
		{
			// If there is a valid collision, find the new world ahead on the track
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