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

/* Create object */
SDKMeshGO3D::SDKMeshGO3D(std::string _filename)
{
	filename = _filename;
}

/* Destroy everything */
SDKMeshGO3D::~SDKMeshGO3D()
{
	m_modelResources.reset();
	m_model.reset();
}

/* Render the model if it exists */
void SDKMeshGO3D::Render()
{
	if (!m_model) {
		//DebugText::print("Call to render non-loaded model: " + filename);
		return;
	}
	if ((m_shouldRender && !isDebugMesh()) || (GameDebugToggles::show_debug_meshes && isDebugMesh())) {
		//Begin scene
		auto commandList = Locator::getDD()->m_deviceResources->GetCommandList();
		Locator::getDD()->m_hdrScene->BeginScene(commandList);

		//Set to our model's heaps
		ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), Locator::getRD()->m_states->Heap() };
		commandList->SetDescriptorHeaps(_countof(heaps), heaps);

		//Load PBR data per PBREffect
		{
			/* 
			Continuing work on custom metal/non-metal render definitions.
			Current only applies to track, but will be extended to all models.
			*/
			auto radianceTex = m_resourceDescriptors->GetGpuHandle(radiance_index);
			auto diffuseDesc = m_radianceIBL->GetDesc();
			auto irradianceTex = m_resourceDescriptors->GetGpuHandle(irradiance_index);

			int i = 0;
			for (auto& it : m_modelNormal)
			{
				auto pbr = dynamic_cast<PBREffect*>(it.get());
				if (pbr)
				{
					//Set IBL textures
					if (current_metalness.size() != 0 && current_metalness.at(i)) {
						//Metal
						pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, Locator::getRD()->m_states->AnisotropicClamp());
					}
					else
					{
						//Non-metal
						pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, radianceTex, Locator::getRD()->m_states->AnisotropicClamp());
					}
					i++;
				}
			}
		}

		//Update effects and draw
		Model::UpdateEffectMatrices(m_modelNormal, m_world, Locator::getRD()->m_cam->GetView(), Locator::getRD()->m_cam->GetProj());
		m_model->Draw(commandList, m_modelNormal.cbegin());

		//End scene
		Locator::getDD()->m_hdrScene->EndScene(commandList);
	}
}

/* Load the model */
void SDKMeshGO3D::Load()
{
	DebugText::print("LOADING MODEL '" + filename + "'!");

	//Our D3D device
	auto device = Locator::getDD()->m_deviceResources->GetD3DDevice();

	//Set the RenderTargetState state to our HDR scene config
	RenderTargetState hdrState(Locator::getDD()->m_hdrScene->GetFormat(), Locator::getDD()->m_deviceResources->GetDepthBufferFormat());

	//Begin the resource upload
	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	//Set the sprite batch description (do we really need this?!)
	{
		SpriteBatchPipelineStateDescription pd(hdrState);
	}

	//Create our resource descriptor - I'm allocating 1024 spaces for now
	m_resourceDescriptors = std::make_unique<DescriptorPile>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		1024,
		0);

	//Find our current env maps
	wchar_t radiance[_MAX_PATH] = {};
	wchar_t irradiance[_MAX_PATH] = {};
	DX::FindMediaFile(radiance, _MAX_PATH, Locator::getRD()->current_cubemap_radiance.c_str());
	DX::FindMediaFile(irradiance, _MAX_PATH, Locator::getRD()->current_cubemap_irradiance.c_str());

	//Create texture from env map files
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, resourceUpload, radiance, m_radianceIBL.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, resourceUpload, irradiance, m_irradianceIBL.ReleaseAndGetAddressOf()));

	//Push radiance texture to resource stores
	radiance_index = resourceDescriptorOffset;
	CreateShaderResourceView(device, m_radianceIBL.Get(), m_resourceDescriptors->GetCpuHandle(radiance_index), true);
	resourceDescriptorOffset++;

	//And the same for irradiance
	irradiance_index = resourceDescriptorOffset;
	CreateShaderResourceView(device, m_irradianceIBL.Get(), m_resourceDescriptors->GetCpuHandle(irradiance_index), true);
	resourceDescriptorOffset++;

	//End the upload and wait for it to finish
	auto uploadResourcesFinished = resourceUpload.End(Locator::getDD()->m_deviceResources->GetCommandQueue());
	Locator::getDD()->m_deviceResources->WaitForGpu();
	uploadResourcesFinished.wait();

	//Set model type - is this actually used anywhere anymore?
	m_type = GO3D_RT_SDK;

	//Work out the filepath to our model
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string fullpath = m_filepath.generateFilepath(filename, m_filepath.MODEL);
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
				DebugText::print("TRIED TO LOAD '" + filename + "' WHICH IS DEPRECIATED (V1) - UPDATE YOUR MODELS FFS!");
				return;
			}
		}

		//Load as SDKMESH
		m_model = Model::CreateFromSDKMESH(modelBin.data(), modelBin.size());
	}
	catch (...)
	{
		//Couldn't load model - not good! Might be trying to load a non-sdkmesh, or have the wrong filepath.
		DebugText::print("TRIED TO LOAD '" + filename + "' BUT IT FAILED. IS IT A SDKMESH?");
		m_model.reset();
		return;
	}

	//Only continue if we loaded our model
	if (m_model)
	{
		//Get current directory (this is currently hacky and needs a fix - is VS giving us the wrong working dir?)
		std::string curr_dir = std::experimental::filesystem::current_path().string();
		if (curr_dir.substr(curr_dir.length() - 6) == "SOURCE") {
			curr_dir = curr_dir.substr(0, curr_dir.length() - 7);
		}
		std::string dirpath = curr_dir + "/" + m_filepath.getFolder(m_filepath.MODEL) + filename + "/";
		if (dirpath.length() > 7 && dirpath.substr(dirpath.length() - 6) == "DEBUG/") {
			dirpath = dirpath.substr(0, dirpath.length() - 7) + "/";
			is_debug_mesh = true;
			DebugText::print("'" + filename + "' IS A DEBUG MESH!");
			#ifndef _DEBUG
			return;
			#endif
		}
		std::wstring dirpath_wstring = std::wstring(dirpath.begin(), dirpath.end());
		const wchar_t* dirpath_wchar = dirpath_wstring.c_str();

		//Create another resource upload
		ResourceUploadBatch resourceUpload(device);
		resourceUpload.Begin();

		//Static buffers and create TextureEffectFactory
		m_model->LoadStaticBuffers(device, resourceUpload, true);
		m_modelResources = std::make_unique<EffectTextureFactory>(device, resourceUpload, m_resourceDescriptors->Heap());

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
			DebugText::print("COULDN'T LOAD MATERIALS FOR MODEL '" + filename + "' - IS THE FILE PATH CORRECT?");
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

			if (!is_debug_mesh) {
				//Load current metalness config
				std::string filepath = m_filepath.getFolder(GameFilepaths::MODEL) + filename + "/REFLECTION.ThICC";
				std::ifstream fin(filepath, std::ios::binary);

				if (fin.good()) {
					//Get number of materials in config
					fin.seekg(0);
					int number_of_materials = 0;
					fin.read(reinterpret_cast<char*>(&number_of_materials), sizeof(int));

					//Go through config for number of materials
					for (int i = 0; i < number_of_materials; i++) {
						char data;
						fin.read(&data, sizeof(bool));
						bool choice = static_cast<bool>(data);
						current_metalness.push_back(choice);
					}
				}
				else
				{
					DebugText::print("Model '" + filename + "' uses a depreciated configuration.");
				}
			}
		}

		//Finish upload and wait for it to end
		auto uploadResourcesFinished = resourceUpload.End(Locator::getDD()->m_deviceResources->GetCommandQueue());
		Locator::getDD()->m_deviceResources->WaitForGpu();
		uploadResourcesFinished.wait();
	}
}

/* Reset all resources */
void SDKMeshGO3D::Reset()
{
	DebugText::print("UN-LOADING MODEL '" + filename + "'!");
	Locator::getDD()->m_deviceResources->WaitForGpu();
	m_modelResources.reset();
	m_model.reset();
	m_modelNormal.clear();
	m_resourceDescriptors.reset();
	resourceDescriptorOffset = 0;
	current_metalness.clear();
}

/* Update */
void SDKMeshGO3D::Tick()
{
	GameObject3D::Tick();
}
