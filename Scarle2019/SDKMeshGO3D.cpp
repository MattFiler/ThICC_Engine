#include "pch.h"
#include "SDKMeshGO3D.h"
#include <codecvt>
#include "RenderData.h"
#include "GameDebugToggles.h"

//The Mesh Content Task of Vis Studio should be able to take fbx, dae and obj models
SDKMeshGO3D::SDKMeshGO3D(RenderData* _RD, string _filename)
{
	m_type = GO3D_RT_SDK;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = m_filepath.generateFilepath(_filename, m_filepath.MODEL);
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	//A crash here means that the model file wasn't loaded properly.
	//Have you got the correct file name?
	m_model = Model::CreateFromSDKMESH(wFilename.c_str());

	ResourceUploadBatch resourceUpload(_RD->m_d3dDevice.Get());

	resourceUpload.Begin();

	string dirpath = m_filepath.getFolder(m_filepath.MODEL) + _filename + "/";
	if (dirpath.length() > 7 && dirpath.substr(dirpath.length() - 6) == "DEBUG/") {
		dirpath = dirpath.substr(0, dirpath.length() - 7) + "/";
		is_debug_mesh = true;
	}
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
		CommonStates::AlphaBlend,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);

	static EffectPipelineStateDescription pdAlpha(
		nullptr,
		CommonStates::AlphaBlend,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);

	m_modelNormal = m_model->CreateEffects(*_RD->m_fxFactory, pd, pdAlpha);
}


SDKMeshGO3D::~SDKMeshGO3D()
{
	m_modelResources.reset();
	m_model.reset();
}

void SDKMeshGO3D::Render(RenderData * _RD)
{
	if (!isDebugMesh() || (GameDebugToggles::show_debug_meshes && isDebugMesh())) {
		ID3D12DescriptorHeap* heaps[] = { m_modelResources->Heap(), _RD->m_states->Heap() };
		_RD->m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
		int uj = 0;
		Model::UpdateEffectMatrices(m_modelNormal, m_world, _RD->m_cam->GetView(), _RD->m_cam->GetProj());
		//for (int i = 0; i < num_of_cam; i++)
		//{
		//	Model::UpdateEffectMatrices(m_modelNormal, m_world, _RD->m_cam[i]->GetView(), _RD->m_cam[i]->GetProj());
		//}
		m_model->Draw(_RD->m_commandList.Get(), m_modelNormal.cbegin());
	}
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
