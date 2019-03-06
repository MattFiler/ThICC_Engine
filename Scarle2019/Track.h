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
	void CompareVectorToMinimum(Vector& _vect);
	void CompareVectorToMaximum(Vector& _vect);

	void SplitTrisIntoGrid();
	void SetAllTrisForIndex(int _index);
	bool IsPointInBounds(Vector& _point, Vector& _lowerBound, Vector& _upperBound);

	Vector GetAreaAtIndex(int _index);
	int GetIndexAtPoint(Vector point);
	void GetXYZIndexAtPoint(Vector& point);

	// Storage for the all the triangles data. 
	// Make sure to not reserve more on this vector past initization, else the references will all get messed up
	std::vector<MeshTri> m_triangles;

	// References to the triangles ordered and split into segments
	std::vector<std::vector<MeshTri*>> m_triGrid;

	GameFilepaths m_filepath;
	TrackData m_track_data;

	// Keep track of the smallest and largest co-ordinates
	Vector m_smallest = Vector3::Zero;
	Vector m_largest = Vector3::Zero;

	// Size for the tri segments (segments are cubes)
	float m_triSegSize = 10;

	// Size of each dimension of the vector
	int m_triGridX = 0;
	int m_triGridY = 0;
	int m_triGridYX = 0; // Set to m_triGridY * m_triGridX as this number is used lots
};