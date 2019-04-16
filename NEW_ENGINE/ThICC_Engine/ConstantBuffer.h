#pragma once

//=================================================================
//constant buffer for the base shader used for the VBGO renderer
//=================================================================

#include <SimpleMath.h>

using namespace DirectX;
using namespace SimpleMath;

struct ConstantBuffer
{
	Matrix	world;
	Matrix	view;
	Matrix	projection;
	Matrix	rot;
	Color	lightCol;
	Color	ambientCol;
	Vector3 lightPos;
};