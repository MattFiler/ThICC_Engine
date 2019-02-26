#pragma once

//=================================================================
// default Vertex Structure used by the VBGO render system
//=================================================================

#include <d3d12.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct myVertex
{
	Vector3 Pos;
	Vector3 Norm;
	Color Color;
	Vector2 texCoord;
};

const D3D12_INPUT_ELEMENT_DESC myVertexLayout[] =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
};