#pragma once

#include "pch.h"
#include "Camera.h"
#include "SDKMeshGO3D.h"
#include <GeometricPrimitive.h>

class Skybox : public GameObject3D {
public:
	Skybox() = default;
	~Skybox();

	void Load() override;
	void Tick(Camera* cam);
	void Tick() override {};
	void Render() override;
	void Reset() override;

	bool Loaded() { return is_loaded; }

private:
	//Skybox geometry
	std::unique_ptr<GeometricPrimitive> skybox;
	std::unique_ptr<BasicEffect> skybox_effect;
	std::unique_ptr<DirectX::DescriptorPile> skybox_resources;
	Microsoft::WRL::ComPtr<ID3D12Resource> skybox_texture;

	bool is_loaded = false;
};