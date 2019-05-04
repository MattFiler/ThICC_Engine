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
	void Render() override;
	void Reset() override;

private:
	//Skybox geometry
	std::unique_ptr<DirectX::DescriptorPile> skybox_resources;
	Microsoft::WRL::ComPtr<ID3D12Resource> skybox_texture;

	SDKMeshGO3D* model = nullptr;
};