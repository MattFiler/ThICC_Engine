#include "pch.h"
#include "TrackMagnet.h"
#include "directxmath.h"
#include "GameStateData.h"


TrackMagnet::TrackMagnet(RenderData* _RD, string _filename) : PhysModel(_RD, _filename)
{
	m_autoCalculateWolrd = false;
	Vector3 scale = Vector3::Zero;
	m_world.Decompose(scale, m_quatRot, m_pos);
}

bool TrackMagnet::ShouldStickToTrack(Track& track, GameStateData* _GSD)
{
	Vector intersect;
	MeshTri* tri = nullptr;
	Matrix targetWorld = Matrix::Identity;
	bool shouldStick = track.DoesLineIntersect(m_world.Down()*5, m_pos + m_world.Up() * 4, intersect, tri);
	if (shouldStick)
	{
		Vector adjustVel = m_velTotal;
		// If velocity is opposite to direction, then the kart is reversing
		if ((m_velTotal + m_world.Forward()).Length() < m_velTotal.Length())
		{
			adjustVel *= -1;
		}
		// TODO: This offset is hard coded, change if the kart ever has bounds?
		Vector offset = m_world.Up() / 4;
		// If the position of the kart is outside of acceptable distance, and within acceptable snapping distance
		float dist = Vector::Distance(m_pos + offset, intersect);
		if (dist > m_minSnapDist && dist < m_maxSnapDist)
		{
			// Turn gravity off when within the snapping zone, this smooths out movment
			m_gravVel = Vector3::Zero;
			m_gravDirection = Vector3::Zero;
			Vector3 moveVector = (intersect + offset) - m_pos;
			if (moveVector.Length() > m_maxSnapSnep* _GSD->m_dt)
			{
				moveVector.Normalize();
				moveVector *= m_maxSnapSnep* _GSD->m_dt;
			}
			SetPos(m_pos + moveVector);
		}
		else if (dist > m_maxSnapDist)
		{
			m_gravDirection = m_world.Down() * 5;
		}

		// Calculate a new rotation using 2 points on the plane that is found
		Vector secondIntersect;
		MeshTri* tri2 = nullptr;
		tri->DoesLineIntersect(m_world.Down() * 5, m_pos + adjustVel + m_world.Forward() + (m_world.Up() * 4), secondIntersect, tri2);
		targetWorld = m_world.CreateWorld(m_pos, secondIntersect - intersect, tri->m_plane.Normal());
		targetWorld = Matrix::CreateScale(m_scale) * targetWorld;
	}
	else
	{
		targetWorld = m_world.CreateWorld(m_pos, m_world.Forward(), m_world.Up());
		targetWorld = Matrix::CreateScale(m_scale) * targetWorld;
		m_gravDirection = m_world.Down() * 5;
	}
	// Update the position and rotation by breaking apart m_world
	Vector3 scale = Vector3::Zero;
	Quaternion rot = Quaternion::Identity;
	targetWorld.Decompose(scale, rot, m_pos);

	// Calculate the distance between the current and target rotation
	Quaternion inverseRot = Quaternion::Identity;
	m_quatRot.Inverse(inverseRot);
	float lerpDelta = (m_maxRotation * _GSD->m_dt) / (inverseRot*rot).Length();
	if (lerpDelta > 1)
	{
		lerpDelta = 1;
	}
	// Lerp the rotation by the calculated amount
	m_quatRot = Quaternion::Lerp(m_quatRot, rot, lerpDelta);

	// Rebuild m_world
	m_rot = Matrix::CreateFromQuaternion(m_quatRot);
	m_world = Matrix::CreateScale(m_scale) * m_rot * Matrix::CreateTranslation(m_pos);

	return shouldStick;
}
