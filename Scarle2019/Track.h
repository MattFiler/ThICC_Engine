#pragma once
#include "PhysModel.h"
#include "MeshTri.h"
#include "GameFilepaths.h"
#include <json.hpp>
using json = nlohmann::json;

struct TrackData {
	float scale = 1.0f;
	Vector3 spawn_pos = Vector3(0, 0, 0);
	Vector3 start_rot = Vector3(0, 0, 0);
};

/* A type of PhysModel that stores a reference to their triangles, used so that TrackMagnet objects
   can calculate the exact point of collision with this mesh. */
class Track : public PhysModel
{
public:
	Track(RenderData* _RD, string _filename);

	Vector3 getSuitableSpawnSpot();

	bool DoesLineIntersect(Vector _direction, Vector _startPos, Vector& _intersect, MeshTri*& _tri);
private:
	void LoadVertexList(string _vertex_list);
	void CreateAndAddTriangle(string _line);
	Vector CreateVector(string _vector);


	std::vector<MeshTri> triangles;

	GameFilepaths m_filepath;
	TrackData m_track_data;
};