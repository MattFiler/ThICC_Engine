#include "pch.h"
#include "GPGO3D.h"
#include "RenderData.h"


GPGO3D::GPGO3D(GPGO3D_Type _type) :m_GPType(_type)
{
	m_type = GO3D_RT_GEOP;
	//TODO:: These all have muchmore parameters, might be a good isdea to try and use them
	switch (m_GPType)
	{
	case GP_CUBE:
		m_shape = GeometricPrimitive::CreateCube();
		break;
	case GP_SPHERE:
		m_shape = GeometricPrimitive::CreateSphere();
		break;
	case GP_GEOSPHERE:
		m_shape = GeometricPrimitive::CreateGeoSphere();
		break;
	case GP_CYLINDER:
		m_shape = GeometricPrimitive::CreateCylinder();
		break;
	case GP_CONE:
		m_shape = GeometricPrimitive::CreateCone();
		break;
	case GP_TORUS:
		m_shape = GeometricPrimitive::CreateTorus();
		break;
	case GP_TETRAHEDRON:
		m_shape = GeometricPrimitive::CreateTetrahedron();
		break;
	case GP_OCTAHEDRON:
		m_shape = GeometricPrimitive::CreateOctahedron();
		break;
	case GP_DODECAHEDRON:
		m_shape = GeometricPrimitive::CreateDodecahedron();
		break;
	case GP_ICOSAHDERON:
		m_shape = GeometricPrimitive::CreateIcosahedron();
		break;
	case GP_TEAPOT:
		m_shape = GeometricPrimitive::CreateTeapot();
		break;
	}

}


GPGO3D::~GPGO3D()
{
	if (m_shape)
	{
		m_shape.reset();
	}
}

void GPGO3D::Render(RenderData * _RD)
{
	//TODO:: See  https://github.com/Microsoft/DirectXTK12/wiki/3D-shapes add textures to these things
	//TODO:: Use the perpixel lighting from there too!

	if (m_GPType != GP_NONE)
	{
		_RD->m_GPeffect->SetMatrices(m_world, _RD->m_cam->GetView(), _RD->m_cam->GetProj());
		_RD->m_GPeffect->Apply(_RD->m_commandList.Get());
		m_shape->Draw(_RD->m_commandList.Get());
	}
}

void GPGO3D::Tick(GameStateData * _GSD)
{
	GameObject3D::Tick(_GSD);
}

void GPGO3D::Reset()
{
	if (m_shape)
	{
		m_shape.reset();
	}
}
