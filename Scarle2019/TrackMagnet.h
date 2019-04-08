#pragma once
#include "Track.h"

class TrackMagnet : public PhysModel
{
public:
	TrackMagnet(string _filename);
	bool ShouldStickToTrack(Track& track);
	void ResolveWallCollisions(Track& walls);
	virtual void Tick() override { PhysModel::Tick(); };

	void setDampenWallReflect(bool _toggle) { m_dampenWallReflect = _toggle; };

protected:
	bool m_onTrack = false;

private:
	void MapVectorOntoTri(Vector& _vect, Vector& _startPos, Vector& _down, MeshTri * _tri);

	float m_minSnapDist = 0.001f; // Under this distance the kart wont snap to the track
	float m_maxSnapDist = 2; // Over this distance the kart wont snap to the track
	float m_maxSnapSnep = 10; // The max distance to snap by each second

	float m_maxAngle = 0.4; // The steepest angle that will be considered a viable floor collision
	float m_maxRotation = 5; // The maximum amount to rotate by each second

	float gravityMultiplier = 35;

	MeshTri* tri = nullptr;

	bool m_dampenWallReflect = true;
};