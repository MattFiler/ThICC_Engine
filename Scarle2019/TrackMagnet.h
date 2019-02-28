#pragma once
#include "Track.h"

class TrackMagnet : public PhysModel
{
public:
	TrackMagnet(RenderData* _RD, string _filename);
	bool ShouldStickToTrack(Track& track);

private:
};