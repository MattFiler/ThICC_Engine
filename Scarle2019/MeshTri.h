#pragma once
#include "pch.h"

using Vector = DirectX::SimpleMath::Vector3;
using Plane = DirectX::SimpleMath::Plane;

/* THIS ENUM SHOULD MATCH THE ONE FOUND WITHIN THE ENGINE'S TOOLKIT: "/EditorTool/AssetTypes.cs" */
enum CollisionType {
	ON_TRACK,
	OFF_TRACK,
	BOOST_PAD,

	NUM_OF_TYPES //must be last
};

struct MeshTri
{
	MeshTri(Vector _a, Vector _b, Vector _c, int _type);
	bool DoesLineIntersect(const Vector& _direction, const Vector& _startPos, Vector& _intersect, MeshTri*& _tri, const float& _maxAngle);
	Vector GetUpperBound();
	Vector GetLowerBound();
	CollisionType GetType() { return m_type; };

	Vector m_pointA;
	Vector m_pointB;
	Vector m_pointC;
	Plane m_plane;
	CollisionType m_type;
};