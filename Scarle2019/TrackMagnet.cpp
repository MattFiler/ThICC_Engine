#include "pch.h"
#include "TrackMagnet.h"


TrackMagnet::TrackMagnet(RenderData* _RD, string _filename) : PhysModel(_RD, _filename)
{

}

bool TrackMagnet::ShouldStickToTrack(Track& track)
{
	Vector intersect;
	bool shouldStick = track.DoesLineIntersect(Vector(0, 10, 0), m_pos, intersect);
	if (shouldStick)
	{
		SetPos(intersect);
	}
	return shouldStick;
}
