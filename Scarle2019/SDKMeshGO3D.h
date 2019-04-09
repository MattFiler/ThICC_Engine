#pragma once
#include "GameObject3D.h"
#include "GameFilepaths.h"
#include <string>
struct RenderData;


class SDKMeshGO3D :
	public GameObject3D
{
public:
	SDKMeshGO3D(string _filename);
	virtual ~SDKMeshGO3D();

	virtual void Render();

	virtual void Tick() override;

	bool isDebugMesh() { return is_debug_mesh; }

	void Reset() override;

protected:
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<DirectX::Model> m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>> m_modelNormal; 

	GameFilepaths m_filepath;

	bool is_debug_mesh = false;
};

