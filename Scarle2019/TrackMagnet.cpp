#include "pch.h"
#include "TrackMagnet.h"
#include "directxmath.h"


TrackMagnet::TrackMagnet(RenderData* _RD, string _filename) : PhysModel(_RD, _filename)
{
	m_autoCalculateWolrd = false;
}

bool TrackMagnet::ShouldStickToTrack(Track& track)
{
	Vector intersect;
	MeshTri* tri = nullptr;
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
		if (dist > minSnapDist && dist < maxSnapDist)
		{
			// Turn gravity off when within the snapping zone, this smooths out movment
			m_gravVel = Vector3::Zero;
			m_gravDirection = Vector3::Zero;
			Vector3 moveVector = (intersect + offset) - m_pos;
			if (moveVector.Length() > maxSnapSnep)
			{
				moveVector.Normalize();
				moveVector *= maxSnapSnep;
			}
			SetPos(m_pos + moveVector);
		}
		else if (dist > maxSnapDist)
		{
			m_gravDirection = m_world.Down() * 5;
		}

		// Calculate a new rotation using 2 points on the plane that is found
		Vector secondIntersect;
		MeshTri* tri2 = nullptr;
		tri->DoesLineIntersect(m_world.Down() * 5, m_pos + adjustVel + m_world.Forward() + (m_world.Up() * 4), secondIntersect, tri2);
		m_world = m_world.CreateWorld(m_pos, secondIntersect - intersect, tri->m_plane.Normal());
		m_world = Matrix::CreateScale(m_scale) * m_world;

	}
	else
	{
		m_world = m_world.CreateWorld(m_pos, m_world.Forward(), m_world.Up());
		m_world = Matrix::CreateScale(m_scale) * m_world;
		m_gravDirection = m_world.Down() * 5;
	}
	// Update the position and rotation by breaking apart m_world
	Vector3 scale = Vector3::Zero;
	Quaternion rot = Quaternion::Identity;
	m_world.Decompose(scale, rot, m_pos);
	m_rot = Matrix::CreateFromQuaternion(rot);

	return shouldStick;
}
