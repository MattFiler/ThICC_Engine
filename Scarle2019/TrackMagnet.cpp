#include "pch.h"
#include "TrackMagnet.h"


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
		Vector offset = m_world.Up() / 4;
		SetPos(intersect + offset); 
		if (m_vel.Distance(Vector(0, 0, 0), m_vel) < 0.1f)
		{
			m_worldMagnet = m_world.CreateWorld(m_pos, m_world.Forward(), tri->m_plane.Normal());
		}
		else
		{
			Vector secondIntersect;
			MeshTri* tri2 = nullptr;
			tri->DoesLineIntersect(m_world.Down() * 5, m_pos + m_vel + m_world.Up() * 2, secondIntersect, tri2);
			m_worldMagnet = m_world.CreateWorld(m_pos, secondIntersect - intersect, tri->m_plane.Normal());
		}
		m_useMagnetMatrix = true;
	}
	else
	{
		m_useMagnetMatrix = false;
		// TODO: Make this change with delta_time
		m_vel += m_world.Down();
	}

	return shouldStick;
}
