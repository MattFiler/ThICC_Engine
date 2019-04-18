#include "pch.h"
#include "SDKMeshGO3D.h"
#include <codecvt>
#include "RenderData.h"
#include "GameDebugToggles.h"
#include "ReadData.h"
#include "SDKMesh.h"
#include "DeviceData.h"
#include "FindMedia.h"
#include <experimental/filesystem>

/* Load model */
SDKMeshGO3D::SDKMeshGO3D(std::string _filename)
{
	//Our D3D device
	auto device = Locator::getDD()->m_deviceResources->GetD3DDevice();

	//Set the HDR state (does this need to be done every time? we do it in our core engine render call)
	RenderTargetState hdrState(Locator::getDD()->m_hdrScene->GetFormat(), Locator::getDD()->m_deviceResources->GetDepthBufferFormat());

	//Begin the resource upload
	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	//Set the sprite batch description (do we really need this?!)
	{
		SpriteBatchPipelineStateDescription pd(hdrState);
	}

	//Create our resource descriptor - I'm allocating 1024 spaces for now
	m_resourceDescriptors = std::make_unique<DescriptorPile>(Locator::getDD()->m_deviceResources->GetD3DDevice(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		1024,
		0);

	//Names of our radiance and irradiance env maps - it'd be nice to load these globally rather than per model :)
	static const wchar_t* s_radianceIBL[(unsigned long long)NUM_OF_ENV_MAPS::ENV_MAP_COUNT] =
	{
		L"DATA/IMPORTED/SunSubMixer_diffuseIBL.dds",
		L"DATA/IMPORTED/Atrium_diffuseIBL.dds",
		L"DATA/IMPORTED/Garage_diffuseIBL.dds",
	};
	static const wchar_t* s_irradianceIBL[(unsigned long long)NUM_OF_ENV_MAPS::ENV_MAP_COUNT] =
	{
		L"DATA/IMPORTED/SunSubMixer_specularIBL.dds",
		L"DATA/IMPORTED/Atrium_specularIBL.dds",
		L"DATA/IMPORTED/Garage_specularIBL.dds",
	};

	//Load the env maps - what a waste!
	for (size_t j = 0; j < (unsigned long long)NUM_OF_ENV_MAPS::ENV_MAP_COUNT; ++j)
	{
		static_assert(_countof(s_radianceIBL) == _countof(s_irradianceIBL), "IBL array mismatch");

		wchar_t radiance[_MAX_PATH] = {};
		wchar_t irradiance[_MAX_PATH] = {};

		DX::FindMediaFile(radiance, _MAX_PATH, s_radianceIBL[j]);
		DX::FindMediaFile(irradiance, _MAX_PATH, s_irradianceIBL[j]);

		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, radiance, m_radianceIBL[j].ReleaseAndGetAddressOf())
		);

		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, resourceUpload, irradiance, m_irradianceIBL[j].ReleaseAndGetAddressOf())
		);

		CreateShaderResourceView(device, m_radianceIBL[j].Get(), m_resourceDescriptors->GetCpuHandle(j), true);
		resourceDescriptorOffset++;
		CreateShaderResourceView(device, m_irradianceIBL[j].Get(), m_resourceDescriptors->GetCpuHandle((int)NUM_OF_ENV_MAPS::ENV_MAP_COUNT + j), true);
		resourceDescriptorOffset++;
	}

	//End the upload and wait for it to finish
	auto uploadResourcesFinished = resourceUpload.End(Locator::getDD()->m_deviceResources->GetCommandQueue());
	Locator::getDD()->m_deviceResources->WaitForGpu();
	uploadResourcesFinished.wait();

	//Set model type - is this actually used anywhere anymore?
	m_type = GO3D_RT_SDK;

	//Work out the filepath to our model
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string fullpath = m_filepath.generateFilepath(_filename, m_filepath.MODEL);
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	//Try load model
	try
	{
		//Load model binary data
		auto modelBin = DX::ReadData(wFilename.c_str());

		//Check SDKMESH version
		if (modelBin.size() >= sizeof(DXUT::SDKMESH_HEADER))
		{
			auto hdr = reinterpret_cast<const DXUT::SDKMESH_HEADER*>(modelBin.data());
			if (!(hdr->Version >= 200))
			{
				//The SDKMESH isn't V2 - we can't load it! This should never happen by the time we get to ship :)
				std::cout << "TRIED TO LOAD '" << _filename << "' WHICH IS DEPRECIATED (V1) - UPDATE YOUR MODELS FFS!" << std::endl;
				return;
			}
		}

		//Load as SDKMESH
		m_model = Model::CreateFromSDKMESH(modelBin.data(), modelBin.size());
	}
	catch (...)
	{
		//Couldn't load model - not good!
		std::cout << "TRIED TO LOAD '" << _filename << "' BUT IT FAILED. IS IT A SDKMESH?" << std::endl;
		m_model.reset();
		return;
	}

	//Only continue if we loaded our model
	if (m_model) 
	{
		//Create another resource upload
		ResourceUploadBatch resourceUpload(device);
		resourceUpload.Begin();

		//Static buffers and create TextureEffectFactory
		m_model->LoadStaticBuffers(device, resourceUpload, true);
		m_modelResources = std::make_unique<EffectTextureFactory>(device, resourceUpload, m_resourceDescriptors->Heap());
	
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

		//Set the directory for our materials
		m_modelResources->SetDirectory(dirpath_wchar);

		try
		{
			//Load materials
			(void)m_model->LoadTextures(*m_modelResources, resourceDescriptorOffset);
		}
		catch (...)
		{
			//Couldn't load model's materials - again not good! We might have the wrong working directory.
			std::cout << "COULDN'T LOAD MATERIALS FOR MODEL '" << _filename << "' - IS THE FILE PATH CORRECT?" << std::endl;
			m_model.reset();
			m_modelResources.reset();
			return;
		}

		if (m_model)
		{
			//Get effect factory - shouldn't this be per model?! It's currently shared around everything which seems a little odd
			IEffectFactory *fxFactory = nullptr;
			Locator::getRD()->m_fxFactoryPBR = std::make_unique<PBREffectFactory>(m_modelResources->Heap(), Locator::getRD()->m_states->Heap());
			fxFactory = Locator::getRD()->m_fxFactoryPBR.get();

			//Opaque materials
			EffectPipelineStateDescription pd(
				nullptr,
				CommonStates::Opaque,
				CommonStates::DepthDefault,
				CommonStates::CullClockwise,
				hdrState);

			//Transparent materials
			EffectPipelineStateDescription pdAlpha(
				nullptr,
				CommonStates::AlphaBlend,
				CommonStates::DepthDefault,
				CommonStates::CullClockwise,
				hdrState);

			//Create effects for materials
			m_modelNormal = m_model->CreateEffects(*fxFactory, pd, pdAlpha, resourceDescriptorOffset);
		}

		//Finish upload and wait for it to end
		auto uploadResourcesFinished = resourceUpload.End(Locator::getDD()->m_deviceResources->GetCommandQueue());
		Locator::getDD()->m_deviceResources->WaitForGpu();
		uploadResourcesFinished.wait();
	}
}

/* Destroy! */
SDKMeshGO3D::~SDKMeshGO3D()
{
	m_modelResources.reset();
	m_model.reset();
}

/* Render the model if it exists */
void SDKMeshGO3D::Render()
{
	if (!m_model) {
		return; //model doesn't exist, we're probably in the process of deleting
	}
	if (m_shouldRender && !isDebugMesh() || (GameDebugToggles::show_debug_meshes && isDebugMesh())) {

		auto commandList = Locator::getDD()->m_deviceResources->GetCommandList();
		Locator::getDD()->m_hdrScene->BeginScene(commandList);

		ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), Locator::getRD()->m_states->Heap() };
		commandList->SetDescriptorHeaps(_countof(heaps), heaps);

		{
			/* This is wrong and what is contributing to the gross visual output. We're getting the GPU handles for the model's materials, not where the env maps are stored :) */
			/* The 2D sprites also don't work due to a similar issue - we're allocating sprites to the wrong descriptor causing memory issues. */
			auto radianceTex = m_resourceDescriptors->GetGpuHandle(Locator::getRD()->m_ibl);
			auto diffuseDesc = m_radianceIBL[Locator::getRD()->m_ibl]->GetDesc();
			auto irradianceTex = m_resourceDescriptors->GetGpuHandle((int)NUM_OF_ENV_MAPS::ENV_MAP_COUNT + Locator::getRD()->m_ibl);

			for (auto& it : m_modelNormal)
			{
				auto pbr = dynamic_cast<PBREffect*>(it.get());
				if (pbr)
				{
					pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, Locator::getRD()->m_states->AnisotropicClamp());
				}
			}
		}

		Model::UpdateEffectMatrices(m_modelNormal, m_world, Locator::getRD()->m_cam->GetView(), Locator::getRD()->m_cam->GetProj());
		m_model->Draw(commandList, m_modelNormal.cbegin());

		Locator::getDD()->m_hdrScene->EndScene(commandList);
	}
}

/* Reset all resources */
void SDKMeshGO3D::Reset()
{
	//This is what I'm probably gonna use for the deletion process - double check it resets everything properly and doesn't leave any lingering resources
	m_modelResources.reset();
	m_model.reset();
	m_modelNormal.clear();
}

/* Update */
void SDKMeshGO3D::Tick()
{
	GameObject3D::Tick();
}
