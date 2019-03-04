#pragma once
#include "Track.h"

class TrackMagnet : public PhysModel
{
public:
	TrackMagnet(RenderData* _RD, string _filename);
	bool ShouldStickToTrack(Track& track, GameStateData* _GSD);

private:
	float m_minSnapDist = 0.001f; // Under this distance the kart wont snap to the track
	float m_maxSnapDist = 5; // Over this distance the kart wont snap to the track
	float m_maxSnapSnep = 40; // The max distance to snap by each second

	Quaternion m_quatRot = Quaternion::Identity;
	float m_maxRotation = 40; // The maximum amount to rotate by each second
};