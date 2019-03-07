#pragma once
#include "Track.h"

class TrackMagnet : public PhysModel
{
public:
	TrackMagnet(RenderData* _RD, string _filename);
	bool ShouldStickToTrack(Track& track, GameStateData* _GSD);
	void ResolveWallCollisions(Track& walls);

private:
	float m_minSnapDist = 0.001f; // Under this distance the kart wont snap to the track
	float m_maxSnapDist = 0.5f; // Over this distance the kart wont snap to the track
	float m_maxSnapSnep = 50; // The max distance to snap by each second

	Quaternion m_quatRot = Quaternion::Identity;
	float m_maxAngle = 0.3f; // The steepest angle that will be considered a viable floor collision
	float m_maxRotation = 15; // The maximum amount to rotate by each second

	float gravityMultiplier = 15;
};