#pragma once
#include "PhysModel.h"
#include "MeshTri.h"
#include "GameFilepaths.h"
#include "ItemBox.h"
#include "DebugMarker.h"
#include "BlenderToDirectX.h"
#include "MapInfo.h"
#include <json.hpp>
#include <map>
using json = nlohmann::json;

struct TrackData {
	float scale = 1.0f;
	Vector3 origin = Vector3(0, 0, 0);
	Vector3 spawn_pos = Vector3(0, 0, 0);
	Vector3 start_rot = Vector3(0, 0, 0);
};

struct Waypoint {
	Waypoint(Vector3 _tl, Vector3 _tr, Vector3 _bl, Vector3 _br, Vector3 _mid) {
		top_left = _tl;
		top_right = _tr;
		bottom_left = _bl;
		bottom_right = _br;
		middle = _mid;
	}
	Vector3 top_left = Vector3(0, 0, 0);
	Vector3 top_right = Vector3(0, 0, 0);
	Vector3 bottom_left = Vector3(0, 0, 0);
	Vector3 bottom_right = Vector3(0, 0, 0);
	Vector3 middle = Vector3(0, 0, 0);
};

struct IntroCamGroup {
	IntroCamGroup() {
		start_pos = Vector3(0, 0, 0);
		end_pos = Vector3(0, 0, 0);
		look_at = Vector3(0, 0, 0);
	}
	IntroCamGroup(Vector3 _start, Vector3 _end, Vector3 _lookat) {
		start_pos = _start;
		end_pos = _end;
		look_at = _lookat;
	}
	Vector3 start_pos;
	Vector3 end_pos;
	Vector3 look_at;
};

/* A type of PhysModel that stores a reference to their triangles, used so that TrackMagnet objects
   can calculate the exact point of collision with this mesh. */
class Track : public PhysModel
{
public:
	Track(MapInfo* _track);

	void LoadCollision();
	void UnloadCollision();

	Vector3 getSuitableSpawnSpot();

	std::vector<Waypoint> getWaypoints() {
		return map_waypoints;
	};
	std::vector<Vector3> getSpawnpoints() {
		return map_spawnpoints;
	};
	std::vector<IntroCamGroup> getIntroCams() {
		return map_intro_cams;
	};
	std::vector<BoundingOrientedBox> getWaypointsBB() {
		return waypoint_bb;
	};
	Waypoint getFinishLine() {
		//This is all stored as a vector, but really there should only be one finish line per map - so just return element zero.
		//If we end up changing maps to have multiple end-points, then this can easily be supported by modifying this return type.
		return map_finishline.at(0);
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

	bool DoesLineIntersect(const Vector& _direction, const Vector& _startPos, Vector& _intersect, MeshTri*& _tri, const float& _maxAngle, const float& _minAngle);
	void SetValidCollision(const bool& _boost, const bool& _off, const bool& _on, const bool& _wall, const bool& _glider, const bool& _antigrav, const bool& _jump, const bool& _noai);

	Vector3 getWaypointMiddle(int index);

	void Render() override;
	void Tick() override;

private:
	void LoadVertexList(std::string _vertex_list);
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
	std::vector<Waypoint> map_waypoints;
	std::vector<Vector3> map_spawnpoints;
	std::vector<IntroCamGroup> map_intro_cams = std::vector<IntroCamGroup>(4);
	std::vector<Vector3> map_itemboxes_pos;
	std::vector<Vector3> map_itemboxes_rot; //broken
	std::vector<Waypoint> map_finishline;

	// waypoint bounding box
	std::vector<BoundingOrientedBox> waypoint_bb;

	//Item boxes
	std::vector<ItemBox*> item_boxes;

	//Debugmarkers
	std::vector<DebugMarker*> debug_markers;

	BlenderToDirectX blender_vector;

	std::string filename;

	// Size of each dimension of the vector
	size_t m_triGridX = 0;
	size_t m_triGridY = 0;
	size_t m_triGridZ = 0;
	size_t m_triGridYX = 0; // Set to m_triGridY * m_triGridX as this number is used lots

	SDKMeshGO3D* m_decoration_model = nullptr;

	std::map<CollisionType, bool> m_validCollisions;
};