#ifndef _PILLOW_H_
#define _PILLOW_H_
#include "VBCube.h"

//=================================================================
//create a puffed out cube VBGO
//=================================================================

class VBPillow : public VBCube
{
public:
	VBPillow(){};

protected:
	virtual void Transform() override
	{
		for (unsigned int i = 0; i<m_numPrims * 3; i++)
		{
			m_vertices[i].Color *= ((i / 3) % 2) ? 1.0f : 0.5f;
			m_vertices[i].Color.w = 1.0f;

			Vector3 vertPos = m_vertices[i].Pos;
			Vector3 spherePos = m_vertices[i].Pos;

			spherePos.Normalize();

			Vector3 newPos = 0.5f * (vertPos + (float)(m_size - 1) * spherePos);

			m_vertices[i].Pos = newPos;
		}
	}

};

#endif