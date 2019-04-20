#pragma once
#include "GameObject3D.h"
#include "GameFilepaths.h"
#include "Constants.h"
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

	void Load() override;
	void Reset() override;

	void SetShouldRender(bool _shouldRender) { m_shouldRender = _shouldRender; };

protected:
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<DirectX::Model> m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>> m_modelNormal;
	std::unique_ptr<DirectX::DescriptorPile>        m_resourceDescriptors;
	Microsoft::WRL::ComPtr<ID3D12Resource>          m_radianceIBL[(unsigned long long)NUM_OF_ENV_MAPS::ENV_MAP_COUNT];
	Microsoft::WRL::ComPtr<ID3D12Resource>          m_irradianceIBL[(unsigned long long)NUM_OF_ENV_MAPS::ENV_MAP_COUNT];

	GameFilepaths m_filepath;

	std::string filename;

	int resourceDescriptorOffset = 0;

	bool is_debug_mesh = false;
	bool m_shouldRender = true;
};

