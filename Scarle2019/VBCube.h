#pragma once
#include "VBGO3D.h"
#include "vertex.h"

//=================================================================
//procedurally generate a VBGO Cube
//each side be divided in to _size * _size squares (2 triangles per square)
//=================================================================

class VBCube : public VBGO3D
{
public:
	VBCube(RenderData * _RD):VBGO3D(_RD){};
	virtual ~VBCube(){};

	//initialise the Veretx and Index buffers for the cube
	void Init(int _size, RenderData* _RD);

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see VBSpiral, VBSpiked and VBPillow
	virtual void Transform(){};

	int m_size;
	myVertex* m_vertices;
};