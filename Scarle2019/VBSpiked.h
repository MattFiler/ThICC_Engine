#pragma once
#include "VBCube.h"

//=================================================================
//create a cube with a spike as a VBGO
//=================================================================

class VBSpike : public VBCube
{
public:
	VBSpike() :VBCube() {};

protected:
	virtual void Transform() override
	{
		for (unsigned int i = 0; i<m_numIndices; i++)
		{
			Vector3 vertPos = m_vertices[i].Pos;

			Matrix transform = Matrix::CreateTranslation(0.0f, 6.0f - sqrt(vertPos.x*vertPos.x + vertPos.z*vertPos.z), 0.0f);

			Vector3 newPos = Vector3::Transform(vertPos, transform);
			
			m_vertices[i].Pos = newPos;
		}
	}

};