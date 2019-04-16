#include "pch.h"
#include "SDKMeshGO3D.h"
#include <codecvt>
#include "RenderData.h"

//The Mesh Content Task of Vis Studio should be able to take fbx, dae and obj models
SDKMeshGO3D::SDKMeshGO3D(RenderData* _RD, string _filename)
{
	m_type = GO3D_RT_SDK;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = "../MARIOKARTSTADIUM/MARIOKARTSTADIUM.SDKMESH";
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	m_model = Model::CreateFromSDKMESH(wFilename.c_str());

	ResourceUploadBatch resourceUpload(_RD->m_d3dDevice.Get());
	resourceUpload.Begin();

	m_model->LoadStaticBuffers(_RD->m_d3dDevice.Get(), resourceUpload, true);

	m_modelResources = std::make_unique<EffectTextureFactory>(_RD->m_d3dDevice.Get(), resourceUpload, _RD->m_resourceDescriptors->Heap());

	m_modelResources->SetDirectory(L"F:/Github Repos/GEP-2019-WorkingAI/NEW_ENGINE/MARIOKARTSTADIUM/");

	(void)m_model->LoadTextures(*m_modelResources, 9);

	_RD->m_fxFactoryPBR = std::make_unique<PBREffectFactory>(m_modelResources->Heap(), _RD->m_states->Heap());

	//RenderTargetState hdrState(m_hdrScene->GetFormat(), m_deviceResources->GetDepthBufferFormat());

	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		//hdrState);
		rtState);

	EffectPipelineStateDescription pdAlpha(
		nullptr,
		CommonStates::AlphaBlend,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		//hdrState);
		rtState);

	m_modelNormal = m_model->CreateEffects(*_RD->m_fxFactoryPBR, pd, pdAlpha, 9);
}


SDKMeshGO3D::~SDKMeshGO3D()
{
	m_modelResources.reset();
	m_model.reset();
}

void SDKMeshGO3D::Render(RenderData * _RD)
{
	ID3D12DescriptorHeap* heaps[] = { m_modelResources->Heap(), _RD->m_states->Heap() };
	_RD->m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	/*
	{
		auto radianceTex = m_resourceDescriptors->GetGpuHandle(Descriptors::RadianceIBL1 + m_ibl);
		auto diffuseDesc = m_radianceIBL[0]->GetDesc();
		auto irradianceTex = m_resourceDescriptors->GetGpuHandle(Descriptors::IrradianceIBL1 + m_ibl);

		for (auto& it : m_gameMapEffects)
		{
			auto pbr = dynamic_cast<PBREffect*>(it.get());
			if (pbr)
			{
				pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, m_states->AnisotropicClamp());
			}
		}
	}
	*/

	Model::UpdateEffectMatrices(m_modelNormal, m_world, _RD->m_cam->GetView(), _RD->m_cam->GetProj());
	m_model->Draw(_RD->m_commandList.Get(), m_modelNormal.cbegin());
}

void SDKMeshGO3D::Reset()
{
	m_modelResources.reset();
	m_model.reset();
	m_modelNormal.clear();
}

void SDKMeshGO3D::Tick(GameStateData * _GSD)
{
	GameObject3D::Tick(_GSD);
}
