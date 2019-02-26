#pragma once
#include "VBGO3D.h"

//=================================================================
//create a VBGO for the isosurface of a given function using the marching cubes
//it was a bit of hack for 3D printing project
//=================================================================


class VBMarchCubes : public VBGO3D
{
public:
	VBMarchCubes(RenderData * _RD);
	virtual ~VBMarchCubes() {};

	//initialise the Veretx and Index buffers for this function
	void init(Vector3 _size, float _isolevel, Vector3 _scale, Vector3 _origin, RenderData* _RD);
	void init(Vector3 _min,Vector3 _max,Vector3 _size, float _isoLevel, RenderData* _RD); //2


protected:
	float function(Vector3 _pos);
};
