#include "pch.h"
#include "TrackMagnet.h"
#include "directxmath.h"


TrackMagnet::TrackMagnet(RenderData* _RD, string _filename) : PhysModel(_RD, _filename)
{

}

bool TrackMagnet::ShouldStickToTrack(Track& track)
{
	Vector intersect;
	MeshTri* tri = nullptr;
	bool shouldStick = track.DoesLineIntersect(m_world.Down()*5, m_pos + m_world.Up() * 2, intersect, tri);
	if (shouldStick)
	{
		Vector adjustVel = m_vel;
		// If velocity is opposite to direction, then the kart is reversing
		if ((m_vel + m_world.Forward()).Length() < m_vel.Length())
		{
			adjustVel *= -1;
		}
		// TODO: This offset is hard coded, change if the kart ever has bounds?
		Vector offset = m_world.Up() / 4;
		// If the position of the kart outside of acceptable distance, and within acceptable snapping distance

		SetPos(intersect + offset);
		// If the karts speed is very low, then don't calculate a new rotation
		if (adjustVel.Distance(Vector(0, 0, 0), adjustVel) < 0.9f)
		{
			m_world = m_world.CreateWorld(m_pos, m_world.Forward(), tri->m_plane.Normal());
		}
		else
		{
			// Calculate a new rotation using 2 points on the plane that is found
			Vector secondIntersect;
			MeshTri* tri2 = nullptr;
			tri->DoesLineIntersect(m_world.Down() * 5, m_pos + adjustVel + m_world.Up() * 2, secondIntersect, tri2);
			m_world = m_world.CreateWorld(m_pos, secondIntersect - intersect, tri->m_plane.Normal());
			m_world = Matrix::CreateScale(m_scale) * m_world;
			Vector3 scale = Vector3(0, 0, 0);
			Quaternion rot = Quaternion::Identity;
			m_world.Decompose(scale, rot, m_pos);
			m_rot = Matrix::CreateFromQuaternion(rot);
		}
		m_autoCalculateWolrd = false;
	}
	else
	{
		m_autoCalculateWolrd = true;
		// TODO: Make this change with delta_time
		m_vel += m_world.Down();
	}

	return shouldStick;
}
