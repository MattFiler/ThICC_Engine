#pragma once
#include "PhysModel.h"
#include "MeshTri.h"

/* A type of PhysModel that stores a reference to their triangles, used so that TrackMagnet objects
   can calculate the exact point of collision with this mesh. */
class Track : public PhysModel
{
public:
	Track(RenderData* _RD, string _filename, string _vertex_list);
	bool DoesLineIntersect(Vector _direction, Vector _startPos, Vector& _intersect);
private:
	void LoadVertexList(string _vertex_list);
	Vector CreateVector(string _line);

	std::vector<MeshTri> triangles;
};