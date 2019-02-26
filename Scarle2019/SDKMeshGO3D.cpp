#include "pch.h"
#include "SDKMeshGO3D.h"
#include <codecvt>
#include "RenderData.h"

//The Mesh Content Task of Vis Studio should be able to take fbx, dae and obj models
SDKMeshGO3D::SDKMeshGO3D(RenderData* _RD, string _filename)
{
	m_type = GO3D_RT_SDK;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = "../Models/" + _filename + "/" + _filename + ".sdkmesh";
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	//A crash here means that the model file wasn't loaded properly.
	//Have you got the correct file name?
	m_model = Model::CreateFromSDKMESH(wFilename.c_str());

	ResourceUploadBatch resourceUpload(_RD->m_d3dDevice.Get());

	resourceUpload.Begin();

	string dirpath = "../Models/" + _filename + "/";
	std::wstring dirpath_wstring = std::wstring(dirpath.begin(), dirpath.end());
	const wchar_t* dirpath_wchar = dirpath_wstring.c_str();

	//A crash here means that the material texture file wasn't loaded properly.
	//Did you utilise my brilliant toolkit properly?
	m_modelResources = m_model->LoadTextures(_RD->m_d3dDevice.Get(), resourceUpload, dirpath_wchar);

	_RD->m_fxFactory = std::make_unique<EffectFactory>(m_modelResources->Heap(), _RD->m_states->Heap());

	auto uploadResourcesFinished = resourceUpload.End(_RD->m_commandQueue.Get());

	uploadResourcesFinished.wait();

	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	static EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);

	static EffectPipelineStateDescription pdAlpha(
		nullptr,
		CommonStates::AlphaBlend,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);

	//TODO: Do we actually need one of these for everyone?
	m_modelNormal = m_model->CreateEffects(*_RD->m_fxFactory, pd, pdAlpha);
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
