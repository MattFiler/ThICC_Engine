#pragma once
#include "PhysModel.h"
#include "MeshTri.h"
#include "GameFilepaths.h"
#include <json.hpp>
using json = nlohmann::json;

/* A type of PhysModel that stores a reference to their triangles, used so that TrackMagnet objects
   can calculate the exact point of collision with this mesh. */
class Track : public PhysModel
{
public:
	Track(RenderData* _RD, string _filename);
	bool DoesLineIntersect(Vector _direction, Vector _startPos, Vector& _intersect, MeshTri*& _tri);
private:
	void LoadVertexList(string _vertex_list);
	void CreateAndAddTriangle(string _line);
	Vector CreateVector(string _vector);


	std::vector<MeshTri> triangles;

	GameFilepaths m_filepath;
	json m_track_data;
};