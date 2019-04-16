#pragma once
#include "VBCube.h"

//=================================================================
//create a twisted cube VBGO
//=================================================================

class VBSpiral : public VBCube
{
public:
	VBSpiral(RenderData * _RD) :VBCube(_RD) {};
protected:
	virtual void Transform() override
	{
		for (int i = 0; i<m_numIndices; i++)
		{
			Vector3 vertPos = m_vertices[i].Pos;

			Matrix rotMat = Matrix::CreateFromYawPitchRoll(XM_PI*(vertPos.y + 5.0f) / 20.0f, 0.0f, 0.0f);
			
			Vector3 newPos = Vector3::Transform(vertPos, rotMat);

			m_vertices[i].Pos = newPos;
		}
	}
};