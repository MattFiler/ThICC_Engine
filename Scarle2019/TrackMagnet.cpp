#include "pch.h"
#include "TrackMagnet.h"


TrackMagnet::TrackMagnet(RenderData* _RD, string _filename) : PhysModel(_RD, _filename)
{

}

bool TrackMagnet::ShouldStickToTrack(Track& track)
{
	Vector intersect;
	MeshTri* tri = nullptr;
	bool shouldStick = track.DoesLineIntersect(m_world.Down(), m_pos, intersect, tri);
	if (shouldStick)
	{
		SetPos(intersect); 
		Vector secondIntersect;
		MeshTri* tri2 = nullptr;
		tri->DoesLineIntersect(m_world.Down(), m_pos + m_vel, secondIntersect, tri2);

		m_worldMagnet = m_world.CreateWorld(m_pos,secondIntersect - intersect, tri->m_plane.Normal());
		m_useMagnetMatrix = true;

	}
	else
	{
		m_useMagnetMatrix = false;
	}

	return shouldStick;
}
