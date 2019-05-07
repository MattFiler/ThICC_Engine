#pragma once
#include "Track.h"

class TrackMagnet : public PhysModel
{
public:
	TrackMagnet(std::string _filename);
	bool ShouldStickToTrack(Track& track);
	bool ResolveWallCollisions(Track& walls);
	virtual void Tick() override { PhysModel::Tick(); };
	virtual void Render() override;

	void setDampenWallReflect(bool _toggle) { m_dampenWallReflect = _toggle; };

	void UseGroundTypes(bool _flag) { m_useGroundTypes = _flag; };
	CollisionType GetGroundType() { return m_colType; };

	bool HasHitWall() { return m_hasHitWall; };
	void ResetHasHitWall(){ m_hasHitWall = false; };

	void UseMagnet(bool _shouldUse) { m_useMagnet = _shouldUse; };

protected:
	bool m_onTrack = false;
	bool m_useGroundTypes = false;
	CollisionType m_colType = ON_TRACK;

private:
	bool TryCollision(Track& _track, const Vector3& _location, MeshTri*& _closestTri, Vector3& _closestPos);
	void MapVectorOntoTri(Vector& _vect, Vector& _startPos, Vector _down, MeshTri * _tri);

	bool m_useMagnet = true;

	float m_minSnapDist = 0.001f; // Under this distance the kart wont snap to the track
	float m_maxSnapDist = 2; // Over this distance the kart wont snap to the track
	float m_maxSnapSnep = 10; // The max distance to snap by each second

	float m_maxAngle = 0.6; // The steepest angle that will be considered a viable floor collision
	float m_minAngle = 0; // The minimum angle for a wall collision against track
	float m_maxRotation = 5; // The maximum amount to rotate by each second

	float gravityMultiplier = 35;

	MeshTri* tri = nullptr;

	bool m_dampenWallReflect = true;

	float m_timeOffTerrain = 0;
	float m_rotationDelay = 1; // Seconds spent off terrain before the kart rotates to world up

	bool m_hasHitWall = false;
};