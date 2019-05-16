#pragma once
#include "GameObject3D.h"
#include "GameFilepaths.h"
#include "Constants.h"
#include "MaterialConfig.h"
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

	bool IsLoaded() { return m_loaded; };
	
	//These functions are intended for skybox use
	void DisableDepthDefault() { enable_depth_default = false; }
	void AlbedoEmissiveOverride(std::wstring path);
	//---

protected:
	//Model resources
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<DirectX::Model> m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>> m_modelNormal;
	std::unique_ptr<DirectX::DescriptorPile> m_resourceDescriptors;

	//Our IBL cubemaps
	Microsoft::WRL::ComPtr<ID3D12Resource> m_radianceIBL;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_irradianceIBL;

	//Optional albedo override
	Microsoft::WRL::ComPtr<ID3D12Resource> m_materialOverride;
	int material_override_index = -1;
	bool material_override_applied = false;

	//Should use DepthDefault? (available to change mainly b/c of skyboxes)
	bool enable_depth_default = true;

	//Material config
	std::vector<MaterialConfig> m_material_config;

	//Engine features
	GameFilepaths m_filepath;

	//Settings and data
	std::string filename;
	int resourceDescriptorOffset = 0;
	int radiance_index = -1;
	int irradiance_index = -1;
	bool is_debug_mesh = false;
	bool m_shouldRender = true;
	bool m_loaded = false;
	int anim_config_version = -1;
};

