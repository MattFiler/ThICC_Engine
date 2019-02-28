#pragma once
#include "Track.h"
#include "pch.h"
#include <iostream>
#include <fstream>

Track::Track(RenderData* _RD, string _filename) : PhysModel(_RD, _filename)
{
	LoadVertexList("../Models/" + _filename + "/" + _filename + ".collmap");
}

/* Takes the list of vertices generated by the model tool, and creates a vector of MeshTri's with it */
void Track::LoadVertexList(string _vertex_list)
{
	std::ifstream file(_vertex_list);
	string line;


	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			CreateAndAddTriangle(line);
		}
	}
}

/* Takes a line from the file and extract a triangle out of it */
void Track::CreateAndAddTriangle(string _line)
{
	Vector vectors[3];
	string value = "";
	int vectorIndex = 0;

	for (char& c : _line)
	{
		if (c == '(' || c == ' ' || (value == "" && c == ','))
		{
			continue;
		}
		else if (c == ')')
		{
			vectors[vectorIndex] = CreateVector(value);
			vectorIndex++;
			value = "";
			// Once 3 vectors have been found, create a triangle
			if (vectorIndex == 3)
			{
				triangles.push_back(MeshTri(vectors[0], vectors[1], vectors[2]));
				vectorIndex = 0;
			}
		}
		else
		{
			value += c;
		}
	}
}

/* Takes a part from the file and extracts the Vector it represents out */
Vector Track::CreateVector(string _vector)
{
	string value = "";
	float values[3];
	int pointIndex = 0;

	for (char& c : _vector)
	{
		// Once a comma is hit, thats the end of this value
		if (c == ',')
		{
			values[pointIndex] = std::stof(value);
			value = "";
			pointIndex++;
		}
		else
		{
			value += c;
		}
	}
	values[2] = std::stof(value);
	return Vector(values[0], values[1], values[2]);
}

/* Checks through all triangles to see if this line intersects any of them. 
   The point of intersecion is stored in _intersect */
bool Track::DoesLineIntersect(Vector _direction, Vector _startPos, Vector& _intersect, Vector& _colNormal)
{
	int index = 0;
	for (MeshTri& tri : triangles)
	{
		if (tri.DoesLineIntersect(_direction, _startPos, _intersect, _colNormal))
		{
			return true;
		}
		index++;
	}
	index = index;
	return false;
}