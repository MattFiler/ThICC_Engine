#include "pch.h"
#include "SDKMeshGO3D.h"
#include <codecvt>
#include "RenderData.h"
#include "GameDebugToggles.h"
//#include <wrl.h>
//#include <d3d11.h>

//The Mesh Content Task of Vis Studio should be able to take fbx, dae and obj models
SDKMeshGO3D::SDKMeshGO3D(string _filename)
{
	m_type = GO3D_RT_SDK;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = m_filepath.generateFilepath(_filename, m_filepath.MODEL);
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	/*
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> defaultTex;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeMap;

	auto effect = std::make_shared<EnvironmentMapEffect>(Locator::getRD()->m_d3dDevice.Get());
	effect->EnableDefaultLighting();
	effect->SetTexture(defaultTex.Get());
	effect->SetEnvironmentMap(cubeMap.Get());
	*/

	//A crash here means that the model file wasn't loaded properly.
	//Have you got the correct file name?
	m_model = Model::CreateFromSDKMESH(wFilename.c_str());

	ResourceUploadBatch resourceUpload(Locator::getRD()->m_d3dDevice.Get());

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
	m_modelResources = m_model->LoadTextures(Locator::getRD()->m_d3dDevice.Get(), resourceUpload, dirpath_wchar);

	Locator::getRD()->m_fxFactory = std::make_unique<EffectFactory>(m_modelResources->Heap(), Locator::getRD()->m_states->Heap());

	auto uploadResourcesFinished = resourceUpload.End(Locator::getRD()->m_commandQueue.Get());

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

	Locator::getRD()->m_fxFactory->EnablePerPixelLighting(true);
	Locator::getRD()->m_fxFactory->EnableNormalMapEffect(true);
	m_modelNormal = m_model->CreateEffects(*Locator::getRD()->m_fxFactory, pd, pdAlpha);

	for (auto& effect : m_modelNormal)
	{
		auto lights = dynamic_cast<IEffectLights*>(effect.get());
		if (lights)
		{
			lights->SetLightEnabled(0, true);
			lights->SetLightDiffuseColor(0, Colors::White);
			//lights->SetLightDiffuseColor(0, Colors::Gold);
			//lights->SetLightDirection(0, Vector3(0, 0, 0));
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
	}
}


SDKMeshGO3D::~SDKMeshGO3D()
{
	m_modelResources.reset();
	m_model.reset();
}

void SDKMeshGO3D::Render()
{
	if (m_shouldRender && !isDebugMesh() || (GameDebugToggles::show_debug_meshes && isDebugMesh())) {
		ID3D12DescriptorHeap* heaps[] = { m_modelResources->Heap(), Locator::getRD()->m_states->Heap() };
		Locator::getRD()->m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

		Model::UpdateEffectMatrices(m_modelNormal, m_world, Locator::getRD()->m_cam->GetView(), Locator::getRD()->m_cam->GetProj());
		m_model->Draw(Locator::getRD()->m_commandList.Get(), m_modelNormal.cbegin());
	}
}

void SDKMeshGO3D::Reset()
{
	m_modelResources.reset();
	m_model.reset();
	m_modelNormal.clear();
}

void SDKMeshGO3D::Tick()
{
	GameObject3D::Tick();
}
