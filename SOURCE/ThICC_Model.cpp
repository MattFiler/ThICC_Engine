#include "pch.h"
#include "ThICC_Model.h"
#include "ServiceLocator.h"

#include "FindMedia.h"
#include "ReadData.h"
#include "SDKMesh.h"

#include <experimental/filesystem>

using namespace DirectX;
using namespace DirectX::SimpleMath;

ThICC_Model::ThICC_Model() {}

void ThICC_Model::load(std::string _filename) {
	reset();

	std::string fullpath = m_filepath.generateFilepath(_filename, m_filepath.MODEL);
	std::wstring fullpath_wstring = std::wstring(fullpath.begin(), fullpath.end());
	const wchar_t* fullpath_wchar = fullpath_wstring.c_str();

	bool issdkmesh2 = false;
	try
	{
		auto modelBin = DX::ReadData(fullpath_wchar);

		if (modelBin.size() >= sizeof(DXUT::SDKMESH_HEADER))
		{
			auto hdr = reinterpret_cast<const DXUT::SDKMESH_HEADER*>(modelBin.data());
			if (hdr->Version >= 200)
			{
				issdkmesh2 = true;
			}
		}

		m_model = Model::CreateFromSDKMESH(modelBin.data(), modelBin.size());
	}
	catch (...)
	{
		//Ideally show some kind of nicer error here
		throw std::exception("Error Loading Model");
		m_model.reset();
	}

	if (m_model)
	{
		auto device = Locator::getGraphicsResources()->m_deviceResources->GetD3DDevice();

		ResourceUploadBatch resourceUpload(device);

		resourceUpload.Begin();

		m_model->LoadStaticBuffers(device, resourceUpload, true);

		m_modelResources = std::make_unique<EffectTextureFactory>(device, resourceUpload, Locator::getGraphicsResources()->m_resourceDescriptors->Heap());

		if (!issdkmesh2)
		{
			m_modelResources->EnableForceSRGB(true);
		}

		//Get current directory (this is currently hacky and needs a fix - is VS giving us the wrong working dir?)
		std::string curr_dir = std::experimental::filesystem::current_path().string();
		if (curr_dir.substr(curr_dir.length() - 6) == "SOURCE") {
			curr_dir = curr_dir.substr(0, curr_dir.length() - 7);
		}
		std::string dirpath = curr_dir + "/" + m_filepath.getFolder(m_filepath.MODEL) + _filename + "/";
		if (dirpath.length() > 7 && dirpath.substr(dirpath.length() - 6) == "DEBUG/") {
			dirpath = dirpath.substr(0, dirpath.length() - 7) + "/";
			//is_debug_mesh = true;
		}
		std::wstring dirpath_wstring = std::wstring(dirpath.begin(), dirpath.end());
		const wchar_t* dirpath_wchar = dirpath_wstring.c_str();

		m_modelResources->SetDirectory(dirpath_wchar);

		int txtOffset = 9;

		try
		{
			(void)m_model->LoadTextures(*m_modelResources, txtOffset);
		}
		catch (...)
		{
			//Ideally show some kind of nicer error here
			throw std::exception("Error Loading Materials");
			m_model.reset();
			m_modelResources.reset();
		}

		if (m_model)
		{
			IEffectFactory *fxFactory = nullptr;
			if (issdkmesh2)
			{
				m_pbrFXFactory = std::make_unique<PBREffectFactory>(m_modelResources->Heap(), Locator::getGraphicsResources()->m_states->Heap());
				fxFactory = m_pbrFXFactory.get();
			}
			else
			{
				m_fxFactory = std::make_unique<EffectFactory>(m_modelResources->Heap(), Locator::getGraphicsResources()->m_states->Heap());
				fxFactory = m_fxFactory.get();
			}

			RenderTargetState hdrState(Locator::getGraphicsResources()->m_hdrScene->GetFormat(), Locator::getGraphicsResources()->m_deviceResources->GetDepthBufferFormat());

			EffectPipelineStateDescription pd(
				nullptr,
				CommonStates::Opaque,
				CommonStates::DepthDefault,
				CommonStates::CullClockwise,
				hdrState);

			EffectPipelineStateDescription pdAlpha(
				nullptr,
				CommonStates::AlphaBlend,
				CommonStates::DepthDefault,
				CommonStates::CullClockwise,
				hdrState);

			m_modelClockwise = m_model->CreateEffects(*fxFactory, pd, pdAlpha, txtOffset);
		}

		auto uploadResourcesFinished = resourceUpload.End(Locator::getGraphicsResources()->m_deviceResources->GetCommandQueue());

		Locator::getGraphicsResources()->m_deviceResources->WaitForGpu();

		uploadResourcesFinished.wait();

		loaded = true;
	}
}

void ThICC_Model::render() {
	if (loaded) {
		{
			auto radianceTex = Locator::getGraphicsResources()->m_resourceDescriptors->GetGpuHandle(Locator::getGraphicsResources()->Descriptors::RadianceIBL1);
			auto diffuseDesc = Locator::getGraphicsResources()->m_radianceIBL[0]->GetDesc();
			auto irradianceTex = Locator::getGraphicsResources()->m_resourceDescriptors->GetGpuHandle(Locator::getGraphicsResources()->Descriptors::IrradianceIBL1);

			for (auto& it : m_modelClockwise)
			{
				auto pbr = dynamic_cast<PBREffect*>(it.get());
				if (pbr)
				{
					pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, Locator::getGraphicsResources()->m_states->AnisotropicClamp());
				}
			}
		}

		Model::UpdateEffectMatrices(m_modelClockwise, Locator::getGraphicsResources()->m_world, Locator::getGraphicsResources()->m_view, Locator::getGraphicsResources()->m_proj);
		m_model->Draw(Locator::getGraphicsResources()->m_deviceResources->GetCommandList(), m_modelClockwise.cbegin());
	}
}

ThICC_Model::~ThICC_Model() {
	m_modelClockwise.clear();
	m_modelResources.reset();
	m_model.reset();
	m_fxFactory.reset();
	m_pbrFXFactory.reset();
}

void ThICC_Model::reset() {
	m_fxFactory.reset();
	m_pbrFXFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_modelClockwise.clear();

	loaded = false;
}