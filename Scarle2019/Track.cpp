#pragma once
#include "Track.h"
#include "pch.h"
#include <iostream>
#include <fstream>

Track::Track(RenderData* _RD, string _filename, string _vertex_list) : PhysModel(_RD, _filename)
{
	LoadVertexList(_vertex_list);
}

void Track::LoadVertexList(string _vertex_list)
{
	std::ifstream file(_vertex_list);
	string line;

	Vector vectors[3];
	int vectorIndex = 0;

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			vectors[vectorIndex] = CreateVector(line);
			vectorIndex++;
			// Once 3 vectors have been found, create a triangle
			if (vectorIndex == 3)
			{
				triangles.push_back(MeshTri(vectors[0], vectors[1], vectors[2]));
				vectorIndex = 0;
			}
		}
	}
}


/* Takes a line from the file and extracts the Vector it represents out */
Vector Track::CreateVector(string _line)
{
	string value = "";
	float values[3];
	int pointIndex = 0;

	for (char& c : _line)
	{
		if (c == ' ')
		{
			// If a whitespace is hit check if any numbers have been added yet,
			// if not, then this is a leading whitespaec so ignore it
			if (value.length() > 0)
			{
				values[pointIndex] = std::stof(value);
				value = "";
				pointIndex++;
			}
		}
		else
		{
			value += c;
		}
	}

	return Vector(values[0], values[1], values[2]);
}

/* Checks through all triangles to see if this line intersects any of them. 
   The point of intersecion is stored in _intersect */
bool Track::DoesLineIntersect(Vector _direction, Vector _startPos, Vector& _intersect)
{
	for (MeshTri& tri : triangles)
	{
		if (tri.DoesLineIntersect(_direction, _startPos, _intersect))
		{
			return true;
		}
	}
	return false;
}