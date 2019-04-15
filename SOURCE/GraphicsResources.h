#pragma once

#include "pch.h"
#include "DeviceResourcesPC.h"
#include "RenderTexture.h"

struct ThICC_GraphicsResources {
	DirectX::SimpleMath::Matrix                     m_world;
	DirectX::SimpleMath::Matrix                     m_view;
	DirectX::SimpleMath::Matrix                     m_proj;

	std::unique_ptr<DX::DeviceResources>            m_deviceResources;
	std::unique_ptr<DX::RenderTexture>              m_hdrScene;

	std::unique_ptr<DirectX::GraphicsMemory>        m_graphicsMemory;
	std::unique_ptr<DirectX::DescriptorPile>        m_resourceDescriptors;
	std::unique_ptr<DirectX::DescriptorHeap>        m_renderDescriptors;
	std::unique_ptr<DirectX::CommonStates>          m_states;

	std::unique_ptr<DirectX::BasicEffect>                                   m_lineEffect;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_lineBatch;

	std::unique_ptr<DirectX::ToneMapPostProcess>    m_toneMapACESFilmic;

	enum Descriptors
	{
		SceneTex,
		RadianceIBL1,
		RadianceIBL2,
		RadianceIBL3,
		IrradianceIBL1,
		IrradianceIBL2,
		IrradianceIBL3,
		Reserve,
		Count = 1024
	};

	const int hdr_count = 3; //this must relate to the arrays below

	Microsoft::WRL::ComPtr<ID3D12Resource>          m_radianceIBL[3];
	Microsoft::WRL::ComPtr<ID3D12Resource>          m_irradianceIBL[3];

	static const wchar_t* s_radianceIBL[3];
	static const wchar_t* s_irradianceIBL[3];
};