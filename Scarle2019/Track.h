#pragma once
#include "PhysModel.h"
#include "MeshTri.h"
#include "GameFilepaths.h"
#include "ItemBox.h"
#include "DebugMarker.h"
#include "BlenderToDirectX.h"
#include <json.hpp>
using json = nlohmann::json;

struct TrackData {
	float scale = 1.0f;
	Vector3 origin = Vector3(0, 0, 0);
	Vector3 spawn_pos = Vector3(0, 0, 0);
	Vector3 start_rot = Vector3(0, 0, 0);
};

struct FinishLine {
	FinishLine(Vector3 _pos, Vector3 _rot) {
		position = _pos;
		rotation = _rot;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 rotation = Vector3(0, 0, 0);
};

/* A type of PhysModel that stores a reference to their triangles, used so that TrackMagnet objects
   can calculate the exact point of collision with this mesh. */
class Track : public PhysModel
{
public:
	Track(string _filename);

	Vector3 getSuitableSpawnSpot();

	std::vector<Vector3> getWaypoints() {
		return map_waypoints;
	};
	std::vector<Vector3> getSpawnpoints() {
		return map_spawnpoints;
	};
	std::vector<Vector3> getCamerasPos() {
		return map_cams_pos;
	};
	std::vector<Vector3> getCamerasRot() {
		return map_cams_rot;
	};
	std::vector<BoundingOrientedBox> getWaypointsBB() {
		return waypoint_bb;
	};
	FinishLine getFinishLine() {
		//This is all stored as a vector, but really there should only be one finish line per map - so just return element zero.
		//If we end up changing maps to have multiple end-points, then this can easily be supported by modifying this return type.
		return FinishLine(map_finishline_pos.at(0), map_finishline_rot.at(0));
	};
	std::vector<Vector3> getItemBoxesPos() {
		return map_itemboxes_pos;
	};
	std::vector<Vector3> getItemBoxesRot() {
		return map_itemboxes_rot;
	};

	void setWaypointBB();

	std::vector<ItemBox*> GetItemBoxes() {
		return item_boxes;
	};

	std::vector<DebugMarker*> GetDebugMarkers() {
		return debug_markers;
	};

	bool DoesLineIntersect(const Vector& _direction, const Vector& _startPos, Vector& _intersect, MeshTri*& _tri, const float& _maxAngle);

private:
	void LoadVertexList(string _vertex_list);
	void CompareVectorToMinimum(Vector& _vect);
	void CompareVectorToMaximum(Vector& _vect);

	void SplitTrisIntoGrid();
	void SetAllTrisForIndex(int _index);
	bool IsPointInBounds(const Vector& _point, const Vector& _lowerBound, const Vector& _upperBound);

	Vector GetAreaAtIndex(int _index);
	int GetIndexAtPoint(Vector point);
	void GetXYZIndexAtPoint(Vector& point);

	// std::clamp apparently doesn't exists, so I'll just make it here
	void Clamp(float& _num, float _min, float _max);

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

	// Map config data from Blender
	std::vector<Vector3> map_waypoints;
	std::vector<Vector3> map_spawnpoints;
	std::vector<Vector3> map_cams_pos;
	std::vector<Vector3> map_cams_rot;
	std::vector<Vector3> map_itemboxes_pos;
	std::vector<Vector3> map_itemboxes_rot;
	std::vector<Vector3> map_finishline_pos;
	std::vector<Vector3> map_finishline_rot;

	// waypoint bounding box
	std::vector<BoundingOrientedBox> waypoint_bb;

	//Item boxes
	std::vector<ItemBox*> item_boxes;

	//Debugmarkers
	std::vector<DebugMarker*> debug_markers;

	BlenderToDirectX blender_vector;

	// Size of each dimension of the vector
	int m_triGridX = 0;
	int m_triGridY = 0;
	int m_triGridZ = 0;
	int m_triGridYX = 0; // Set to m_triGridY * m_triGridX as this number is used lots
};