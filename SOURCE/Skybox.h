#pragma once

#include "pch.h"
#include "SDKMeshGO3D.h"
#include <GeometricPrimitive.h>

class Skybox {
public:
	Skybox();
	~Skybox();

	void Render();

private:
	//Our IBL cubemaps
	Microsoft::WRL::ComPtr<ID3D12Resource> m_radianceIBL;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_irradianceIBL;

	std::unique_ptr<GeometricPrimitive> skybox;
};