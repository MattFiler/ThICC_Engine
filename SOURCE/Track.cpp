#pragma once
#include "pch.h"
#include "math.h"
#include "Track.h"
#include "DebugMarker.h"
#include "RenderData.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>

Track::Track(MapInfo* _track) : PhysModel(_track->model)
{
	//Read in track config
	std::ifstream i(m_filepath.generateConfigFilepath(_track->model, m_filepath.MODEL));
	json m_track_data_j;
	m_track_data_j << i;

	//Parse loaded config
	m_track_data.scale = m_track_data_j["modelscale"];
	m_track_data.origin = Vector3(m_track_data_j["start_x"], m_track_data_j["start_y"], m_track_data_j["start_z"]); //This is unused atm!
	m_track_data.start_rot = Vector3(m_track_data_j["rot_x"], m_track_data_j["rot_y"], m_track_data_j["rot_z"]); //Hmm, allow this? Will mess with collision.
	m_triSegSize = m_track_data_j["segment_size"];

	//Load decoration model
	m_decoration_model = new SDKMeshGO3D(_track->decoration_model);

	//Parse loaded arrays from config
	for (json::iterator it = m_track_data_j["map_waypoints"].begin(); it != m_track_data_j["map_waypoints"].end(); ++it) {
		Vector3 top_left = blender_vector.ConvertPosition(Vector3(it.value()["top_left"][0], it.value()["top_left"][1], it.value()["top_left"][2]) * m_track_data.scale);
		Vector3 top_right = blender_vector.ConvertPosition(Vector3(it.value()["top_right"][0], it.value()["top_right"][1], it.value()["top_right"][2]) * m_track_data.scale);
		Vector3 bottom_left = blender_vector.ConvertPosition(Vector3(it.value()["bottom_left"][0], it.value()["bottom_left"][1], it.value()["bottom_left"][2]) * m_track_data.scale);
		Vector3 bottom_right = blender_vector.ConvertPosition(Vector3(it.value()["bottom_right"][0], it.value()["bottom_right"][1], it.value()["bottom_right"][2]) * m_track_data.scale);
		Vector3 middle_bottom = bottom_left + ((bottom_right - bottom_left)*0.5f);
		Vector3 middle_top = top_left + ((top_right - top_left)*0.5f);
		Vector3 middle = middle_bottom + ((middle_top - middle_bottom)*0.5f);

		Waypoint new_waypoint = Waypoint(top_left, top_right, bottom_left, bottom_right, middle);
		map_waypoints.push_back(new_waypoint);

		//Calculate debug marker position as the mid-point of top left and bottom right!
		//DebugMarker* new_marker = new DebugMarker(pos, Vector3(0, 0, 0));
		//debug_markers.push_back(new_marker);
	}
	for (json::iterator it = m_track_data_j["map_cameras"].begin(); it != m_track_data_j["map_cameras"].end(); ++it) {
		if (it.value()["role"] == "Start") {
			map_intro_cams.at(it.value()["index"]).start_pos = (blender_vector.ConvertPosition(Vector3((float)it.value()["pos"][0], (float)it.value()["pos"][1], (float)it.value()["pos"][2])) * m_track_data.scale);
		}
		else if (it.value()["role"] == "End") {
			map_intro_cams.at(it.value()["index"]).end_pos = (blender_vector.ConvertPosition(Vector3((float)it.value()["pos"][0], (float)it.value()["pos"][1], (float)it.value()["pos"][2])) * m_track_data.scale);
		}
	}
	if (!m_track_data_j["look_at_points"].is_null()) {
		for (json::iterator it = m_track_data_j["look_at_points"].begin(); it != m_track_data_j["look_at_points"].end(); ++it) {
			map_intro_cams.at(it.value()["index"]).look_at = (blender_vector.ConvertPosition(Vector3((float)it.value()["pos"][0], (float)it.value()["pos"][1], (float)it.value()["pos"][2])) * m_track_data.scale);
		}
	}
	else
	{
		DebugText::print(" >>> This map uses an outdated config! Please re-export for new camera configurations!! <<< ");
	}
	for (json::iterator it = m_track_data_j["map_spawnpoints"].begin(); it != m_track_data_j["map_spawnpoints"].end(); ++it) {
		map_spawnpoints.push_back(blender_vector.ConvertPosition(Vector3(it.value()[0], it.value()[1], it.value()[2]) * m_track_data.scale));
	}
	
	for (json::iterator it = m_track_data_j["map_itemboxes"].begin(); it != m_track_data_j["map_itemboxes"].end(); ++it) {
		Vector3 box_pos = blender_vector.ConvertPosition(Vector3(it.value()["pos"][0], it.value()["pos"][1], it.value()["pos"][2]) * m_track_data.scale);
		Vector3 box_rot = blender_vector.ConvertAngle(Vector3(it.value()["rotation"][0], it.value()["rotation"][1], it.value()["rotation"][2]));

		map_itemboxes_pos.push_back(box_pos);
		map_itemboxes_rot.push_back(box_rot);

		ItemBox* new_item_box = new ItemBox(box_pos, box_rot);
		item_boxes.push_back(new_item_box);
	}
	for (json::iterator it = m_track_data_j["map_finishline"].begin(); it != m_track_data_j["map_finishline"].end(); ++it) {
		Vector3 top_left = blender_vector.ConvertPosition(Vector3(it.value()["top_left"][0], it.value()["top_left"][1], it.value()["top_left"][2]) * m_track_data.scale);
		Vector3 top_right = blender_vector.ConvertPosition(Vector3(it.value()["top_right"][0], it.value()["top_right"][1], it.value()["top_right"][2]) * m_track_data.scale);
		Vector3 bottom_left = blender_vector.ConvertPosition(Vector3(it.value()["bottom_left"][0], it.value()["bottom_left"][1], it.value()["bottom_left"][2]) * m_track_data.scale);
		Vector3 bottom_right = blender_vector.ConvertPosition(Vector3(it.value()["bottom_right"][0], it.value()["bottom_right"][1], it.value()["bottom_right"][2]) * m_track_data.scale);
		Vector3 middle_bottom = bottom_left + ((bottom_right - bottom_left)*0.5f);

		Waypoint finish_line = Waypoint(top_left, top_right, bottom_left, bottom_right, middle_bottom);
		map_finishline.push_back(finish_line);

		//Calculate debug marker position as the mid-point of top left and bottom right!
		//DebugMarker* new_marker = new DebugMarker(pos, rot);
		//debug_markers.push_back(new_marker);
	}

	//Work out the spawn pos from blender definition, or fall back to our origin
	if (map_spawnpoints.size() > 0) {
		m_track_data.spawn_pos = map_spawnpoints.at(0);
	}
	else
	{
		m_track_data.spawn_pos = m_track_data.origin;
	}

	//Set our config in action
	SetScale(m_track_data.scale);
	SetRotationInDegrees(m_track_data.start_rot);

	//Debug output
	DebugText::print("Loaded data for track: " + _track->model);
	//DebugText::print("Suitable spawn spot: (" + std::to_string(m_track_data.spawn_pos.x) + ", " + std::to_string(m_track_data.spawn_pos.y) + ", " + std::to_string(m_track_data.spawn_pos.z) + ")");
	filename = _track->model;
}

/* Load the track collision info */
void Track::LoadCollision() {
	//Load track vertex list for generating our collmap
	DebugText::print("Starting generation of track tri map for '" + filename + "'...");
	LoadVertexList(m_filepath.generateFilepath(filename, m_filepath.MODEL_COLLMAP));

	// Populate the collsion map
	using pair = std::pair<CollisionType, bool>;
	m_validCollisions.insert(pair(CollisionType::ON_TRACK, true));
	m_validCollisions.insert(pair(CollisionType::OFF_TRACK, true));
	m_validCollisions.insert(pair(CollisionType::BOOST_PAD, true));
	m_validCollisions.insert(pair(CollisionType::WALL, true));
	m_validCollisions.insert(pair(CollisionType::GLIDER_TRACK, true));
	m_validCollisions.insert(pair(CollisionType::ANTIGRAV_PAD, true));
	m_validCollisions.insert(pair(CollisionType::JUMP_PAD, true));
	m_validCollisions.insert(pair(CollisionType::ON_TRACK_NO_AI, true));

	//Load decoration model if we have one
	if (m_decoration_model != nullptr) {
		m_decoration_model->Load();
	}
}

/* Unload the track collision info and decoration mesh if we have one */
void Track::UnloadCollision() {
	m_validCollisions.clear();
	m_triangles.clear();
	m_triGrid.clear();
	if (m_decoration_model != nullptr) {
		m_decoration_model->Reset();
	}
	DebugText::print("Unloaded track tri map for '" + filename + "'.");
}

/* Returns a suitable spawn location for a player in this map */
Vector3 Track::getSuitableSpawnSpot() {
	return m_track_data.spawn_pos;
}

/* Takes the list of vertices generated by the model tool, and creates a vector of MeshTri's with it */
void Track::LoadVertexList(std::string _vertex_list)
{
	//Open file stream
	std::ifstream fin(_vertex_list, std::ios::binary);

	//Get the number of collision groups
	fin.seekg(0);
	int number_of_coll_types = 0;
	fin.read(reinterpret_cast<char*>(&number_of_coll_types), sizeof(int));

	//Get the offsets for the vertex groups
	std::vector<int> length(number_of_coll_types);
	for (int i = 0; i < number_of_coll_types; i++) {
		fin.read(reinterpret_cast<char*>(&length[i]), sizeof(int));
	}

	//Read by offsets
	std::vector<std::vector<float>> verts(number_of_coll_types);
	int iterator = 0;
	for (int len : length) {
		if (len == 0) {
			iterator++;
			continue;
		}
		verts.at(iterator) = std::vector<float>(len);
		fin.read(reinterpret_cast<char*>(&verts[iterator][0]), len * sizeof(float));
		iterator++;
	}

	//Create MeshTris for each collision type
	float points_for_triangle[9];
	int index = 0;
	for (int x = 0; x < number_of_coll_types; x++) {
		for (size_t i = 0; i < verts.at(x).size(); ++i)
		{
			points_for_triangle[index] = verts.at(x)[i];
			index++;
			if (index == 9) {
				Vector3 point_1 = Vector3(points_for_triangle[0], points_for_triangle[1], points_for_triangle[2]) * m_track_data.scale;
				CompareVectorToMaximum(point_1);
				CompareVectorToMinimum(point_1);

				Vector3 point_2 = Vector3(points_for_triangle[3], points_for_triangle[4], points_for_triangle[5]) * m_track_data.scale;
				CompareVectorToMaximum(point_2);
				CompareVectorToMinimum(point_2);

				Vector3 point_3 = Vector3(points_for_triangle[6], points_for_triangle[7], points_for_triangle[8]) * m_track_data.scale;
				CompareVectorToMaximum(point_3);
				CompareVectorToMinimum(point_3);

				if (x == 4)
				{
					point_1 = blender_vector.ConvertPosition(point_1);
					point_2 = blender_vector.ConvertPosition(point_2);
					point_3 = blender_vector.ConvertPosition(point_3);
				}

				m_triangles.push_back(MeshTri(point_1, point_2, point_3, x));

				if (x == 4)
				{
					m_triangles.back().m_plane.Normal(m_triangles.back().m_plane.Normal()*-1);
				}
				index = 0;
			}
		}
		DebugText::print("Track collision group " + std::to_string(x) + " has " + std::to_string(verts.at(x).size()) + " entries.");
	}

	//Split triangles up into the grid
	//Eventually this will be done at build time!
	SplitTrisIntoGrid();
}

/* Sets up the bounding boxes for each waypoint */
void Track::setWaypointBB()
{
	for (size_t i = 0; i < map_finishline.size(); ++i)
	{
		Vector3 vertices[8] =
		{
			Vector3(static_cast<float>(map_finishline[i].top_left.x), static_cast<float>(map_finishline[i].top_left.y), static_cast<float>(map_finishline[i].top_left.z)),
			Vector3(static_cast<float>(map_finishline[i].top_right.x), static_cast<float>(map_finishline[i].top_right.y), static_cast<float>(map_finishline[i].top_right.z)),
			Vector3(static_cast<float>(map_finishline[i].bottom_left.x), static_cast<float>(map_finishline[i].bottom_left.y), static_cast<float>(map_finishline[i].bottom_left.z)),
			Vector3(static_cast<float>(map_finishline[i].bottom_right.x), static_cast<float>(map_finishline[i].bottom_right.y), static_cast<float>(map_finishline[i].bottom_right.z)),
			Vector3(static_cast<float>(map_finishline[i].top_left.x+1), static_cast<float>(map_finishline[i].top_left.y + 1), static_cast<float>(map_finishline[i].top_left.z + 1)),
			Vector3(static_cast<float>(map_finishline[i].top_right.x + 1), static_cast<float>(map_finishline[i].top_right.y + 1), static_cast<float>(map_finishline[i].top_right.z + 1)),
			Vector3(static_cast<float>(map_finishline[i].bottom_left.x + 1), static_cast<float>(map_finishline[i].bottom_left.y + 1), static_cast<float>(map_finishline[i].bottom_left.z + 1)),
			Vector3(static_cast<float>(map_finishline[i].bottom_right.x + 1), static_cast<float>(map_finishline[i].bottom_right.y + 1), static_cast<float>(map_finishline[i].bottom_right.z + 1))
		};
		BoundingOrientedBox box;
		BoundingOrientedBox::CreateFromPoints(box, 8, (const XMFLOAT3*)&vertices[0], 3 * sizeof(float));

		waypoint_bb.push_back(box);
	}
	for (size_t i = 0; i < map_waypoints.size(); ++i)
	{
		Vector3 vertices[4] =
		{
			Vector3(static_cast<float>(map_waypoints[i].top_left.x), static_cast<float>(map_waypoints[i].top_left.y), static_cast<float>(map_waypoints[i].top_left.z)),
			Vector3(static_cast<float>(map_waypoints[i].top_right.x), static_cast<float>(map_waypoints[i].top_right.y), static_cast<float>(map_waypoints[i].top_right.z)),
			Vector3(static_cast<float>(map_waypoints[i].bottom_left.x), static_cast<float>(map_waypoints[i].bottom_left.y), static_cast<float>(map_waypoints[i].bottom_left.z)),
			Vector3(static_cast<float>(map_waypoints[i].bottom_right.x), static_cast<float>(map_waypoints[i].bottom_right.y), static_cast<float>(map_waypoints[i].bottom_right.z))
		};
		BoundingOrientedBox box;
		BoundingOrientedBox::CreateFromPoints(box, 4, (const XMFLOAT3*)&vertices[0], 3 * sizeof(float));

		waypoint_bb.push_back(box);
	}
	int y = 0;
}

/* Checks through all triangles to see if this line intersects any of them.
   The point of intersecion is stored in _intersect */
bool Track::DoesLineIntersect(const Vector& _direction, const Vector& _startPos, Vector& _intersect, MeshTri*& _tri, const float& _maxAngle, const float& _minAngle)
{
	// Check to see if the position is within the grid
	if (!IsPointInBounds(_startPos, m_smallest, m_largest))
	{
		return false;
	}
	// Find the bounding box created by _startPos and _startPos + _direction
	Vector endPos = _startPos + _direction;
	Vector upper = Vector(_startPos.x > endPos.x ? _startPos.x : endPos.x,
		_startPos.y > endPos.y ? _startPos.y : endPos.y,
		_startPos.z > endPos.z ? _startPos.z : endPos.z);
	Vector lower = Vector(_startPos.x < endPos.x ? _startPos.x : endPos.x,
		_startPos.y < endPos.y ? _startPos.y : endPos.y,
		_startPos.z < endPos.z ? _startPos.z : endPos.z);
	GetXYZIndexAtPoint(upper);
	GetXYZIndexAtPoint(lower);


	// Then check all the grid sections covered by this area
	MeshTri* closestTri = nullptr;
	float bestDist = 100000;
	Vector closestIntersect = Vector::Zero;
	for (int i = lower.z; i <= upper.z; i++)
	{
		for (int j = lower.y; j <= upper.y; j++)
		{
			size_t index = (i*m_triGridYX) + (j*m_triGridX);
			for (int k = lower.x; k <= upper.x; k++)
			{
				for (MeshTri* tri : m_triGrid[index + k])
				{
					if (m_validCollisions[tri->GetType()] && tri->DoesLineIntersect(_direction, _startPos, _intersect, _tri, _maxAngle, _minAngle))
					{
						if (tri->m_type == GLIDER_TRACK)
						{
							bool breakme = false;
						}
						float dist = Vector::Distance(_startPos, _intersect);
						if (dist < bestDist)
						{
							closestIntersect = _intersect;
							bestDist = dist;
							closestTri = _tri;
						}
					}
				}
			}
		}
	}
	if (closestTri)
	{
		_intersect = closestIntersect;
		_tri = closestTri;
		return true;
	}
	else
	{
		return false;
	}
}

/* Compares the passed vector to the maximum vector member variable, then updates x, y, z if any are bigger */
void Track::CompareVectorToMaximum(Vector& _vect)
{
	if (_vect.x > m_largest.x)
	{
		m_largest.x = _vect.x;
	}
	if (_vect.y > m_largest.y)
	{
		m_largest.y = _vect.y;
	}
	if (_vect.z > m_largest.z)
	{
		m_largest.z = _vect.z;
	}
}

/* Compares the passed vector to the minimum vector member variable, then updates x, y, z if any are smaller */
void Track::CompareVectorToMinimum(Vector& _vect)
{
	if (_vect.x < m_smallest.x)
	{
		m_smallest.x = _vect.x;
	}
	if (_vect.y < m_smallest.y)
	{
		m_smallest.y = _vect.y;
	}
	if (_vect.z < m_smallest.z)
	{
		m_smallest.z = _vect.z;
	}
}

/* Takes the vector of triangles and creates a vector of vectors of references that are 
ordered by position and grouped into a grid space (defined by m_triSegSize) */
void Track::SplitTrisIntoGrid()
{
	Vector trackSize = m_largest - m_smallest;
	// Reserve space in the references vector based on the size of the track
	m_triGridX = static_cast<size_t>(ceilf(trackSize.x / m_triSegSize));
	m_triGridY = static_cast<size_t>(ceilf(trackSize.y / m_triSegSize));
	m_triGridZ = static_cast<size_t>(ceilf(trackSize.z / m_triSegSize));
	m_triGridYX = m_triGridY * m_triGridX;

	m_triGrid.reserve((m_triGridX + 1)*(m_triGridY + 1)*(m_triGridZ + 1));

	for (size_t i = 0; i < m_triGrid.capacity(); i++)
	{
		std::vector<MeshTri*> vec;
		m_triGrid.push_back(vec);
	}
	for (MeshTri& tri : m_triangles)
	{
		if (tri.m_type == GLIDER_TRACK)
		{
			bool breakme = false;
		}
		Vector upper = tri.GetUpperBound();
		Vector lower = tri.GetLowerBound();
		GetXYZIndexAtPoint(upper);
		GetXYZIndexAtPoint(lower);

		for (int i = lower.z; i <= upper.z; i++)
		{
			for (int j = lower.y; j <= upper.y; j++)
			{
				size_t index = (i*m_triGridYX) + (j*m_triGridX);
				for (int k = lower.x; k <= upper.x; k++)
				{
					m_triGrid[index + k].push_back(&tri);
				}
			}
		}
	}

	DebugText::print("Track tri map created with " + std::to_string(m_triGridX * m_triGridY * m_triGridZ) + " segments!");
}

/* Loops though the m_trianges vector and finds every tri that falls within the area at _index  */
void Track::SetAllTrisForIndex(int _index)
{
	Vector startVec = GetAreaAtIndex(_index);
	Vector endVec = startVec + Vector(m_triSegSize, m_triSegSize, m_triSegSize);

	for (MeshTri& tri : m_triangles)
	{
		if (IsPointInBounds(tri.m_pointA, startVec, endVec) ||
			IsPointInBounds(tri.m_pointB, startVec, endVec) ||
			IsPointInBounds(tri.m_pointC, startVec, endVec))
		{
			m_triGrid[_index].push_back(&tri);
		}
	}
}

/* Returns the bottom corner for the grid section this _index represents */
Vector Track::GetAreaAtIndex(int _index)
{
	Vector returnVec = Vector3::Zero;
	returnVec.z = m_triSegSize * floorf(_index / m_triGridYX);
	_index = _index % m_triGridYX;
	returnVec.y = m_triSegSize * floorf(_index / m_triGridX);
	_index = _index % m_triGridX;
	returnVec.x = m_triSegSize * _index;

	return returnVec + m_smallest;
}

/* Returns true if the passed point is inside the bounding box created by _lowerBound and _upperBound */
bool Track::IsPointInBounds(const Vector& _point, const Vector& _lowerBound, const Vector& _upperBound)
{
	return (((_point.x >= _lowerBound.x) && (_point.x <= _upperBound.x)) &&
		((_point.y >= _lowerBound.y) && (_point.y <= _upperBound.y)) &&
		((_point.z >= _lowerBound.z) && (_point.z <= _upperBound.z)));
}

/* Finds the index in the m_triGrid that has this point in its area */
int Track::GetIndexAtPoint(Vector point)
{
	point -= m_smallest;
	point.x = floor(point.x / m_triSegSize);
	point.y = floor(point.y / m_triSegSize);
	point.z = floor(point.z / m_triSegSize);

	return static_cast<int>((point.z * m_triGridYX) + (point.y * m_triGridX) + point.x);
}

/* Similar to GetIndexAtPoint but instead returns the x,y and z indicies as if it were a 3D vector 
   (used in calculating where tri's go in the grid) */
void Track::GetXYZIndexAtPoint(Vector& _point)
{
	_point -= m_smallest;
	_point.x = floor(_point.x / m_triSegSize);
	_point.y = floor(_point.y / m_triSegSize);
	_point.z = floor(_point.z / m_triSegSize);

	Clamp(_point.x, 0.0f, (float)m_triGridX);
	Clamp(_point.y, 0.0f, (float)m_triGridY);
	Clamp(_point.z, 0.0f, (float)m_triGridZ);
}

void Track::Clamp(float& _num, float _min, float _max)
{
	if (_num < _min)
	{
		_num = _min;
	}
	else if (_num > _max)
	{
		_num = _max;
	}
}

void Track::SetValidCollision(const bool& _boost, const bool& _off, const bool& _on, const bool& _wall, const bool& _glider, const bool& _antigrav, const bool& _jump, const bool& _noai)
{
	m_validCollisions[CollisionType::BOOST_PAD] = _boost;
	m_validCollisions[CollisionType::OFF_TRACK] = _off;
	m_validCollisions[CollisionType::ON_TRACK] = _on;
	m_validCollisions[CollisionType::WALL] = _wall;
	m_validCollisions[CollisionType::GLIDER_TRACK] = _glider;
	m_validCollisions[CollisionType::ANTIGRAV_PAD] = _antigrav;
	m_validCollisions[CollisionType::JUMP_PAD] = _jump;
	m_validCollisions[CollisionType::ON_TRACK_NO_AI] = _noai;
}

Vector3 Track::getWaypointMiddle(int index)
{
	while (index >= map_waypoints.size())
	{
		index -= map_waypoints.size();
	}
	return map_waypoints[index].middle;
}

//Allow our decoration model to update and render if we have one
void Track::Render() {
	if (m_decoration_model != nullptr) {
		m_decoration_model->Render();
	}
	PhysModel::Render();
}
void Track::Tick() {
	if (m_decoration_model != nullptr) {
		m_decoration_model->Tick();
	}
	PhysModel::Tick();
}