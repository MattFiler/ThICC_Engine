#pragma once
#include "pch.h"

using Vector = DirectX::SimpleMath::Vector3;
using Plane = DirectX::SimpleMath::Plane;

/* THIS ENUM SHOULD MATCH THE ONE FOUND WITHIN THE ENGINE'S TOOLKIT: "/EditorTool/AssetTypes.cs" */
enum CollisionType {
	ON_TRACK,
	OFF_TRACK,
	BOOST_PAD,
	WALL,

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


	// All of the variable used for mesh collsion are stored here instead of being
	// created on the stack to speed it up
	float angle = 0;
	float numerator = 0;
	float denominator = 0;
	float dist = 0;
	float d = 0;

	Vector u = Vector::Zero;
	Vector v = Vector::Zero;
	Vector n = Vector::Zero;
	Vector w = Vector::Zero;

	float gamma = 0;
	float beta = 0;
	float alpha = 0;
};