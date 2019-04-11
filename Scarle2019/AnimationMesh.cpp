#include "AnimationMesh.h"
#include "pch.h"

AnimationMesh::AnimationMesh(string _filename) :SDKMeshGO3D(_filename)
{
}

void AnimationMesh::Update(Matrix _parentWorld, Vector3 _rotOffsetOverride)
{
	if (!m_posAnimPoints.empty())
	{
		m_posTimeElapsed += Locator::getGSD()->m_dt;
		if (m_posTimeElapsed > m_timeBetweenPos)
		{
			m_posTimeElapsed = m_timeBetweenPos;
		}
		m_posOffset = Vector3::Lerp(m_prevPoint, m_posAnimPoints.front(), m_posTimeElapsed / m_timeBetweenPos);
		if (Vector3::Distance(m_posOffset, m_posAnimPoints.front()) < 0.1f)
		{
			m_posTimeElapsed = 0;
			m_posOffset = m_posAnimPoints.front();
			m_prevPoint = m_posAnimPoints.front();
			m_posAnimPoints.pop();
		}
	}

	if (!m_rotAnimPoints.empty())
	{
		m_rotTimeElapsed += Locator::getGSD()->m_dt;
		if (m_rotTimeElapsed > m_timeBetweenRot)
		{
			m_rotTimeElapsed = m_timeBetweenRot;
		}
		switch (m_rotAnimPoints.front())
		{
		case LEFT:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Left(), m_rotTimeElapsed/m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Left());
			break;
		}
		case BACKWARD:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Backward(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Backward());
			break;
		}
		case RIGHT:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Right(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Right());
			break;
		}
		case FORWARD:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Forward(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Forward());
			break;
		}
		case UP:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Up(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Up());
			break;
		}
		case DOWN:
		{
			m_rotOffset = Vector3::Lerp(GetRotationFromDirection(m_prevDirection, _parentWorld), _parentWorld.Down(), m_rotTimeElapsed / m_timeBetweenRot);
			PopRotationQueue(_parentWorld.Down());
			break;
		}
		}

		 //std::cout << m_rotOffset.x << ", " << m_rotOffset.y << ", " << m_rotOffset.z << std::endl;
	}
	else
	{
		m_rotOffset = Vector3::Lerp(m_rotOffset, _rotOffsetOverride, m_rotAnimSpeed * Locator::getGSD()->m_dt);
	}

	SetWorld(_parentWorld);

	SetWorld(Matrix::CreateScale(m_scale) * Matrix::CreateWorld(m_pos, m_rotOffset, m_world.Up()));
	
	m_pos += m_posOffset;
	UpdateWorld();
}

void AnimationMesh::Jump(float _jumpHeight, float _duration)
{
	if (m_posAnimPoints.empty())
	{
		m_timeBetweenPos = _duration / 2;
		m_posTimeElapsed = 0;
		Vector3 m_prevPoint = Vector3::Zero;
		m_posAnimPoints.push(m_world.Up() * _jumpHeight);
		m_posAnimPoints.push(Vector3::Zero);
	}
}

void AnimationMesh::Spin(int _revolutions, float _duration)
{
	if (m_rotAnimPoints.empty())
	{
		m_timeBetweenRot = _duration / (4 * _revolutions);
		m_rotTimeElapsed = 0;
		direction m_prevDirection = FORWARD;
		for (int i = 0; i < _revolutions; i++)
		{
			m_rotAnimPoints.push(LEFT);
			m_rotAnimPoints.push(BACKWARD);
			m_rotAnimPoints.push(RIGHT);
			m_rotAnimPoints.push(FORWARD);
		}
	}
}

void AnimationMesh::Flip(int _revolutions, float _duration)
{
	if (m_rotAnimPoints.empty())
	{
		m_timeBetweenRot = _duration / (4 * _revolutions);
		m_rotTimeElapsed = 0;
		direction m_prevDirection = FORWARD;
		for (int i = 0; i < _revolutions; i++)
		{
			m_rotAnimPoints.push(DOWN);
			m_rotAnimPoints.push(BACKWARD);
			m_rotAnimPoints.push(UP);
			m_rotAnimPoints.push(FORWARD);
		}
	}
}

void AnimationMesh::PopRotationQueue(Vector3 _targetRot)
{
	if (Vector3::Distance(m_rotOffset, _targetRot) < 0.1f)
	{
		m_rotTimeElapsed = 0;
		m_rotOffset = _targetRot;
		m_prevDirection = m_rotAnimPoints.front();
		m_rotAnimPoints.pop();
	}
}

Vector3 AnimationMesh::GetRotationFromDirection(direction _prevDirection, Matrix _world)
{
	switch (_prevDirection)
	{
	case LEFT:
	{
		return _world.Left();
	}
	case BACKWARD:
	{
		return _world.Backward();
	}
	case RIGHT:
	{
		return _world.Right();
	}
	case FORWARD:
	{
		return _world.Forward();
	}
	case UP:
	{
		return _world.Up();
	}
	case DOWN:
	{
		return _world.Down();
	}
	}
}
