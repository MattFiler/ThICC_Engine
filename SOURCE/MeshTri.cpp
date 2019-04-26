#include "pch.h"
#include "MeshTri.h"
#include <iostream>

MeshTri::MeshTri(Vector _a, Vector _b, Vector _c, int _type) : 
	m_pointA(_a), 
	m_pointB(_b), 
	m_pointC(_c), 
	m_type((CollisionType)_type)
{
	m_plane = Plane(_a, _b, _c);
};

/* Returns true if the given vector, starting at the given position intersects this triangle*/
bool MeshTri::DoesLineIntersect(const Vector& _direction, const Vector& _startPos, Vector& _intersect, MeshTri*& _tri, const float& _maxAngle, const float& _minAngle)
{
	angle = acos((_direction*-1).Dot(m_plane.Normal()) / ((_direction*-1).Length() * m_plane.Normal().Length()));
	if (angle > _maxAngle || angle < _minAngle)
	{
		return false;
	}

	if (Vector::Distance(_direction*-1, m_plane.Normal()) > 2)
	{
		//DebugText::print(std::to_string(Vector::Distance(_direction*-1, m_plane.Normal())));
		//return false;
	}
	// If the normal is pointing away from the _starPos, ignore this tri
	if (_maxAngle < 3 && (m_plane.Normal() + _direction).LengthSquared() > _direction.LengthSquared())
	{
		return false;
	}
	_tri = this;
	// Using the vector equasion for a plane as (p-p0).n = 0 
	// and the vector equasion for a line as p = dl + l0
	// Substituting one into the other, and rearring to find d
	// d = (p0 - l0).n / l.n
	numerator = m_plane.Normal().Dot((m_pointA - _startPos));
	// If the numerator is 0, then the line lies inside the plane
	if (numerator == 0)
	{
		_intersect = _startPos;
		return true;
	}
	denominator = _direction.Dot(m_plane.Normal());
	// If the denominator is 0, then the line is parralel
	if (denominator == 0)
	{
		return false;
	}

	// D here is the scalar for the vector, which determins where on the line is the intersect
	d = numerator / denominator;

	_intersect = d * _direction + _startPos;

	// Check if the intersect is within range of the vector
	dist = Vector::DistanceSquared(_startPos, _intersect);
	if (dist > _direction.LengthSquared())
	{
		return false;
	}

	/* Information for this found at https://gamedev.stackexchange.com/questions/28781/easy-way-to-project-point-onto-triangle-or-plane/152476#152476
	referencing the work done by:
	Wolfgang Heidrich, 2005, Computing the Barycentric Coordinates of a Projected Point, Journal of Graphics Tools, pp 9-12, 10(3).
	I won't pretend that I understand this part at all. */
	// u=P2−P1
	u = m_pointB - m_pointA;
	// v=P3−P1
	v = m_pointC - m_pointA;
	// n=u×v
	n = u.Cross(v);
	// w=P−P1
	w = _intersect - m_pointA;
	// Barycentric coordinates of the projection P′of P onto T:
	// γ=[(u×w)⋅n]/n²
	gamma = u.Cross(w).Dot(n) / n.Dot(n);
	// β=[(w×v)⋅n]/n²
	beta = w.Cross(v).Dot(n) / n.Dot(n);
	alpha = 1 - gamma - beta;
	// The point P′ lies inside T if:

	return ((0 <= alpha) && (alpha <= 1) &&
		(0 <= beta) && (beta <= 1) &&
		(0 <= gamma) && (gamma <= 1));


	return false;
}

// Returns a vector that represents the upper corner of an axis-alighned bounding box containing this triangles
Vector MeshTri::GetLowerBound()
{
	Vector returnVec = m_pointA;
	returnVec.x = returnVec.x < m_pointB.x ? returnVec.x : m_pointB.x;
	returnVec.x = returnVec.x < m_pointC.x ? returnVec.x : m_pointC.x;
	returnVec.y = returnVec.y < m_pointB.y ? returnVec.y : m_pointB.y;
	returnVec.y = returnVec.y < m_pointC.y ? returnVec.y : m_pointC.y;
	returnVec.z = returnVec.z < m_pointB.z ? returnVec.z : m_pointB.z;
	returnVec.z = returnVec.z < m_pointC.z ? returnVec.z : m_pointC.z;

	return returnVec;
}

// Returns a vector that represents the lower corner of an axis-alighned bounding box containing this triangles
Vector MeshTri::GetUpperBound()
{
	Vector returnVec = m_pointA;
	returnVec.x = returnVec.x > m_pointB.x ? returnVec.x : m_pointB.x;
	returnVec.x = returnVec.x > m_pointC.x ? returnVec.x : m_pointC.x;
	returnVec.y = returnVec.y > m_pointB.y ? returnVec.y : m_pointB.y;
	returnVec.y = returnVec.y > m_pointC.y ? returnVec.y : m_pointC.y;
	returnVec.z = returnVec.z > m_pointB.z ? returnVec.z : m_pointB.z;
	returnVec.z = returnVec.z > m_pointC.z ? returnVec.z : m_pointC.z;

	return returnVec;
}