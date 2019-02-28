#include "MeshTri.h"
#include "pch.h"


MeshTri::MeshTri(Vector _a, Vector _b, Vector _c) : m_pointA(_a), m_pointB(_b), m_pointC(_c)
{
	 m_plane = Plane(_a, _b, _c); 
};

/* Returns true if the given vector, starting at the given position intersects this triangle*/
bool MeshTri::DoesLineIntersect(Vector _direction, Vector _startPos, Vector& _intersect, Vector& _colNormal)
{
	_colNormal = m_plane.Normal();
	// Using the vector equasion for a plane as (p-p0).n = 0 
	// and the vector equasion for a line as p = dl + l0
	// Substituting one into the other, and rearring to find d
	// d = (p0 - l0).n / l.n
	float numerator = m_plane.Normal().Dot((m_pointA - _startPos));
	// If the numerator is 0, then the line lies inside the plane
	if (numerator == 0)
	{
		_intersect = _startPos;
		return true;
	}
	float denominator = _direction.Dot(m_plane.Normal());
	// If the denominator is 0, then the line is parralel
	if (denominator == 0)
	{
		return false;
	}

	// D here is the scalar for the vector, which determins where on the line is the intersect
	float d = numerator / denominator;

	_intersect = d*_direction + _startPos;

	// Check if the intersect is within range of the vector
	float dist = Vector::Distance(_startPos, _intersect);
	if (dist > _direction.Length())
	{
		return false;
	}
	
	/* Information for this found at https://gamedev.stackexchange.com/questions/28781/easy-way-to-project-point-onto-triangle-or-plane/152476#152476
	referencing the work done by:
	Wolfgang Heidrich, 2005, Computing the Barycentric Coordinates of a Projected Point, Journal of Graphics Tools, pp 9-12, 10(3).
	I won't pretend that I understand this part at all. */

	// u=P2−P1
	Vector u = m_pointB - m_pointA;
	// v=P3−P1
	Vector v = m_pointC - m_pointA;
	// n=u×v
	Vector n = u.Cross(v);
	// w=P−P1
	Vector w = _intersect - m_pointA;
	// Barycentric coordinates of the projection P′of P onto T:
	// γ=[(u×w)⋅n]/n²
	float gamma = u.Cross(w).Dot(n) / n.Dot(n);
	// β=[(w×v)⋅n]/n²
	float beta = w.Cross(v).Dot(n) / n.Dot(n);
	float alpha = 1 - gamma - beta;
	// The point P′ lies inside T if:
	return ((0 <= alpha) && (alpha <= 1) &&
		(0 <= beta) && (beta <= 1) &&
		(0 <= gamma) && (gamma <= 1));
}