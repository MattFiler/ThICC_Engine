#pragma once
#include "pch.h"

using Vector = DirectX::SimpleMath::Vector3;
using Plane = DirectX::SimpleMath::Plane;

struct MeshTri
{
	MeshTri(Vector _a, Vector _b, Vector _c);
	bool DoesLineIntersect(Vector _direction, Vector _startPos, Vector& _intersect);

	Vector m_pointA;
	Vector m_pointB;
	Vector m_pointC;
	Plane m_plane;
};