#pragma once
#include "GameObject3D.h"

//GEP:: Basic 3D geometric primitives
// https://github.com/Microsoft/DirectXTK12/wiki/3D-shapes

enum GPGO3D_Type
{
	GP_NONE = 0,
	GP_CUBE,
	//TODO:GP_BOX
	GP_SPHERE,
	GP_GEOSPHERE,
	GP_CYLINDER,
	GP_CONE,
	GP_TORUS,
	GP_TETRAHEDRON,
	GP_OCTAHEDRON,
	GP_DODECAHEDRON,
	GP_ICOSAHDERON,
	GP_TEAPOT,
	//TODO: GP_CUSTOM
	//TODO: allow for data to be put in to customise all these
	GP_COUNT

};
class GPGO3D :
	public GameObject3D
{
public:
	GPGO3D(GPGO3D_Type _type = GP_NONE);
	virtual ~GPGO3D();

	virtual void Render(RenderData* _RD);

	virtual void Tick(GameStateData* _GSD) override;

	void Reset() override;

protected:

	std::unique_ptr<DirectX::GeometricPrimitive> m_shape = nullptr;
	GPGO3D_Type m_GPType;
};

