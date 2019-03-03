#pragma once
#include "Track.h"

class TrackMagnet : public PhysModel
{
public:
	TrackMagnet(RenderData* _RD, string _filename);
	bool ShouldStickToTrack(Track& track);

private:
	float minSnapDist = 0.001f; // Under this distance the kart wont snap to the track
	float maxSnapDist = 2; // Over this distance the kart wont snap to the track
	float maxSnapSnep = 0.4f; // The max distance to snap by each frame
};