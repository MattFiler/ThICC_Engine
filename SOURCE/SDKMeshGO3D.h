#pragma once
#include "GameObject3D.h"
#include "GameFilepaths.h"
#include <string>
struct RenderData;


class SDKMeshGO3D :
	public GameObject3D
{
public:
	SDKMeshGO3D(std::string _filename);
	virtual ~SDKMeshGO3D();

	virtual void Render();

	virtual void Tick() override;

	bool isDebugMesh() { return is_debug_mesh; }

	void Reset() override;

	void SetShouldRender(bool _shouldRender) { m_shouldRender = _shouldRender; };

protected:
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<DirectX::Model> m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>> m_modelNormal;
	std::unique_ptr<DirectX::DescriptorPile>        m_resourceDescriptors;

	GameFilepaths m_filepath;

	bool is_debug_mesh = false;
	bool m_shouldRender = true;
};
