#include "pch.h"
#include "SDKMeshGO3D.h"
#include <codecvt>
#include "RenderData.h"
#include "GameDebugToggles.h"
#include "ReadData.h"
#include "SDKMesh.h"
#include "DeviceData.h"
#include "FindMedia.h"
#include "Constants.h"
#include <experimental/filesystem>

/* Create object */
SDKMeshGO3D::SDKMeshGO3D(std::string _filename)
{
	filename = _filename;

	if (_filename.length() > 5 && _filename.substr(_filename.length() - 5) == "DEBUG") {
		return;
	}

	//Fetch config
	std::ifstream x(m_filepath.generateConfigFilepath(_filename, m_filepath.MODEL));
	nlohmann::json model_config;
	model_config << x;

	//Apply config
	SetScale(model_config["modelscale"]);
	SetPos(Vector3(model_config["start_x"], model_config["start_y"], model_config["start_z"]));
	SetRotationInDegrees(Vector3(model_config["rot_x"], model_config["rot_y"], model_config["rot_z"]));
	UpdateWorld();
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
		DebugText::print("Call to render non-loaded model: " + filename);
		return;
	}
	if ((m_shouldRender && !isDebugMesh()) || (GameDebugToggles::show_debug_meshes && isDebugMesh())) {
		//Begin scene
		auto commandList = Locator::getDD()->m_deviceResources->GetCommandList();
		Locator::getDD()->m_hdrScene->BeginScene(commandList);

		//Set to our model's heaps (including anim textures)
		ID3D12DescriptorHeap* heaps[] = { m_resourceDescriptors->Heap(), Locator::getRD()->m_states->Heap() };
		commandList->SetDescriptorHeaps(_countof(heaps), heaps);

		//Load material configs per PBREffect (including IBL)
		{
			auto radianceTex = m_resourceDescriptors->GetGpuHandle(radiance_index);
			auto diffuseDesc = m_radianceIBL->GetDesc();
			#ifndef _ARCADE
			auto irradianceTex = m_resourceDescriptors->GetGpuHandle(irradiance_index);
			#endif

			int i = 0;
			for (auto& it : m_modelNormal)
			{
				auto pbr = dynamic_cast<PBREffect*>(it.get());
				if (pbr)
				{
					#ifdef _ARCADE
					pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, radianceTex, Locator::getRD()->m_states->AnisotropicClamp());
					#else
					/* METALLIC CONFIG */
					if (m_material_config.size() != 0 && m_material_config.at(i).is_metallic) {
						//Metal
						pbr->SetConstantMetallic(1.0f);
						pbr->SetConstantRoughness(0.0f);
						pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, irradianceTex, Locator::getRD()->m_states->AnisotropicClamp());
					}
					else
					{
						//Non-metal
						pbr->SetConstantMetallic(0.0f);
						pbr->SetConstantRoughness(1.0f);
						pbr->SetIBLTextures(radianceTex, diffuseDesc.MipLevels, radianceTex, Locator::getRD()->m_states->AnisotropicClamp());
					}
					/* ANIMATION CONFIG */
					if (anim_config_version == 2) {
						//Config version 2 (outdated)
						if (m_material_config.size() != 0 && m_material_config.at(i).is_animated) {
							MaterialConfig& this_mat = m_material_config.at(i);
							pbr->SetAlbedoTexture(m_resourceDescriptors->GetGpuHandle(this_mat.gpu_indexes.at(this_mat.current_anim_index)));
							if (this_mat.animation_timer > this_mat.animation_time) {
								this_mat.current_anim_index++;
								if (this_mat.texture_names.size() == this_mat.current_anim_index) {
									this_mat.current_anim_index = 0;
								}
								this_mat.animation_timer = 0;
							}
							m_material_config.at(i).animation_timer += (float)Locator::getGSD()->m_dt;
						}
					}
					/* MATERIAL OVERRIDE (ALBEDO + EMISSIVE) */
					if (material_override_index != -1 && !material_override_applied) {
						pbr->SetAlbedoTexture(m_resourceDescriptors->GetGpuHandle(material_override_index));
						pbr->SetEmissiveTexture(m_resourceDescriptors->GetGpuHandle(material_override_index));
					}
					#endif
					i++;
				}
			}
			if (material_override_index != -1 && !material_override_applied) {
				material_override_applied = true;
			}
		}

		//Update effects and draw
		Model::UpdateEffectMatrices(m_modelNormal, m_world, Locator::getRD()->m_cam->GetView(), Locator::getRD()->m_cam->GetProj());
		try {
			m_model->Draw(Locator::getDD()->m_deviceResources->GetCommandList(), m_modelNormal.cbegin());
		}
		catch (...) {
			DebugText::print("Failed to draw model: '" + filename + "'");
		}

		//End scene
		Locator::getDD()->m_hdrScene->EndScene(commandList);
	}
}

/* Override every albedo material for this model (indended for skybox use) */
void SDKMeshGO3D::AlbedoEmissiveOverride(std::wstring path) {
	if (resourceDescriptorOffset < 10) {
		resourceDescriptorOffset += m_modelNormal.size() * 4;
	}
	auto device = Locator::getDD()->m_deviceResources->GetD3DDevice();
	ResourceUploadBatch resourceUpload(device);
	try {
		resourceUpload.Begin();
	}
	catch (...) {
		DebugText::print("SDKMeshGO3D::AlbedoEmissiveOverride - Failed to begin resource upload, we are probably out of memory. Fatal!");
		return;
	}
	wchar_t override_tex[_MAX_PATH] = {};
	DX::FindMediaFile(override_tex, _MAX_PATH, path.c_str());
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, resourceUpload, override_tex, m_materialOverride.ReleaseAndGetAddressOf()));
	CreateShaderResourceView(device, m_materialOverride.Get(), m_resourceDescriptors->GetCpuHandle(resourceDescriptorOffset));
	material_override_index = resourceDescriptorOffset;
	resourceDescriptorOffset++;
	auto uploadResourcesFinished = resourceUpload.End(Locator::getDD()->m_deviceResources->GetCommandQueue());
	uploadResourcesFinished.wait();
}

/* Load the model */
void SDKMeshGO3D::Load()
{
	//Our D3D device
	auto device = Locator::getDD()->m_deviceResources->GetD3DDevice();

	//Set the RenderTargetState state to our HDR scene config
	RenderTargetState hdrState(Locator::getDD()->m_hdrScene->GetFormat(), Locator::getDD()->m_deviceResources->GetDepthBufferFormat());

	//Begin the resource upload
	ResourceUploadBatch resourceUpload(device);
	try {
		resourceUpload.Begin();
	}
	catch (...) {
		DebugText::print("SDKMeshGO3D::Load - Failed to begin resource upload, we are probably out of memory. Fatal!");
		return;
	}

	//Set the sprite batch description (do we really need this?!)
	{
		SpriteBatchPipelineStateDescription pd(hdrState);
	}

	//Create our resource descriptor - I'm allocating 1024 spaces for now
	try {
		m_resourceDescriptors = std::make_unique<DescriptorPile>(device,
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
			1024,
			0);
	}
	catch (...) {
		DebugText::print("SDKMeshGO3D::Load - Failed to allocate 1024 spaces to descriptor pile. Probably out of memory. Fatal!");
		return;
	}

	#ifdef _ARCADE
	//Find env map and load
	wchar_t radiance[_MAX_PATH] = {};
	DX::FindMediaFile(radiance, _MAX_PATH, Locator::getRD()->arcade_cubemap.c_str());
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, resourceUpload, radiance, m_radianceIBL.ReleaseAndGetAddressOf()));

	//Push radiance texture to resource stores
	radiance_index = resourceDescriptorOffset;
	CreateShaderResourceView(device, m_radianceIBL.Get(), m_resourceDescriptors->GetCpuHandle(radiance_index), true);
	resourceDescriptorOffset++;
	#else
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
	#endif

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
				DebugText::print("TRIED TO LOAD '" + filename + "' WHICH IS DEPRECIATED (V1) - UPDATE YOUR MODELS!");
				return;
			}
		}

		//Load as SDKMESH
		m_model = Model::CreateFromSDKMESH(modelBin.data(), modelBin.size());
	}

	//Catch any issues when loading the model
	catch (const std::runtime_error& re)
	{
		std::string error(re.what());
		DebugText::print("RUNTIME ERROR WHILE LOADING '" + filename + "': " + error);
		m_model.reset();
		return;
	}
	catch (const std::exception& ex)
	{
		std::string error(ex.what());
		DebugText::print("EXCEPTION WHILE LOADING '" + filename + "': " + error);
		if (error == "Out of memory.") {
			DebugText::print(" >>>> THIS MODEL SHOULD BE REMOVED FROM THE GAME! IT IS TOO LARGE TO HANDLE! <<<< ");
		}
		m_model.reset();
		return;
	}
	catch (...)
	{
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

			if (enable_depth_default) {
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
			else
			{
				//Opaque materials
				EffectPipelineStateDescription pd(
					nullptr,
					CommonStates::Opaque,
					CommonStates::DepthNone,
					CommonStates::CullClockwise,
					hdrState);

				//Transparent materials
				EffectPipelineStateDescription pdAlpha(
					nullptr,
					CommonStates::AlphaBlend,
					CommonStates::DepthNone,
					CommonStates::CullClockwise,
					hdrState);

				//Create effects for materials
				m_modelNormal = m_model->CreateEffects(*fxFactory, pd, pdAlpha, resourceDescriptorOffset);
			}

			#ifndef _ARCADE
			//Load our engine configs
			if (!is_debug_mesh) {
				/* Load metalness config */
				std::string filepath = m_filepath.getFolder(GameFilepaths::MODEL) + filename + "/REFLECTION.ThICC";
				std::ifstream metal_config(filepath, std::ios::binary);
				if (metal_config.good()) {
					//Verify our config is legit
					bool config_is_legit = false;
					metal_config.seekg(0);
					int config_version = 0;
					metal_config.read(reinterpret_cast<char*>(&config_version), sizeof(int));
					if (config_version == ThICC_File::ThICC_FILE_VERSION || config_version == 2) {
						char* read_confirmation_identifier = new char[ThICC_File::ThICC_FILE_IDENTIFIER.length()];
						metal_config.read(read_confirmation_identifier, ThICC_File::ThICC_FILE_IDENTIFIER.length());
						std::string this_signature;
						this_signature.append(read_confirmation_identifier, ThICC_File::ThICC_FILE_IDENTIFIER.length());
						if (ThICC_File::ThICC_FILE_IDENTIFIER == this_signature) {
							config_is_legit = true;
						}
					}
					if (config_version == 2) {
						DebugText::print("'" + filename + "' uses metal config version 2 - this will soon be depreciated, please re-export the model!");
					}
					else if (config_version != ThICC_File::ThICC_FILE_VERSION) {
						DebugText::print("'" + filename + "' uses a DEPRECIATED metal config (sub-v2)! Please update it by re-exporting the model.");
					}

					if (config_is_legit) {
						//Get number of materials in config
						int number_of_materials = 0;
						metal_config.read(reinterpret_cast<char*>(&number_of_materials), sizeof(int));

						//Go through config for number of materials
						for (int i = 0; i < number_of_materials; i++) {
							char data;
							metal_config.read(&data, sizeof(bool));

							MaterialConfig this_mat;
							this_mat.material_index = i;
							this_mat.is_metallic = static_cast<bool>(data);
							m_material_config.push_back(this_mat);
						}
					}
					else
					{
						DebugText::print("WARNING: MODEL '" + filename + "' USES A DEPRECIATED METAL CONFIGURATION.");
					}
				}
				else
				{
					DebugText::print("WARNING: MODEL '" + filename + "' DOES NOT HAVE A METAL CONFIGURATION.");
				}

				/* Load animation config */
				filepath = m_filepath.getFolder(GameFilepaths::MODEL) + filename + "/ANIMATION.ThICC";
				std::ifstream anim_config(filepath, std::ios::binary); 
				if (anim_config.good()) {
					//Verify our config is legit
					bool config_is_legit = false;
					anim_config.seekg(0);
					anim_config.read(reinterpret_cast<char*>(&anim_config_version), sizeof(int));
					/* We currently support V2 and V3, will soon be V3 only! */
					if (anim_config_version == ThICC_File::ThICC_FILE_VERSION || anim_config_version == 2) { 
						char* read_confirmation_identifier = new char[ThICC_File::ThICC_FILE_IDENTIFIER.length()];
						anim_config.read(read_confirmation_identifier, ThICC_File::ThICC_FILE_IDENTIFIER.length());
						std::string this_signature;
						this_signature.append(read_confirmation_identifier, ThICC_File::ThICC_FILE_IDENTIFIER.length());
						if (ThICC_File::ThICC_FILE_IDENTIFIER == this_signature) {
							config_is_legit = true;
						}
					}
					if (anim_config_version == 2) {
						DebugText::print("'" + filename + "' uses animation config version 2 - this will soon be depreciated, please re-export the model!");
					}
					else if (anim_config_version != ThICC_File::ThICC_FILE_VERSION) {
						DebugText::print("'" + filename + "' uses a DEPRECIATED animation config (sub-v2)! Please update it by re-exporting the model.");
					}

					if (config_is_legit) {
						//Get number of materials in config
						int number_of_materials = 0;
						anim_config.read(reinterpret_cast<char*>(&number_of_materials), sizeof(int));
						if (anim_config_version == ThICC_File::ThICC_FILE_VERSION) { //V3 is the new norm, but wrap it for V2 configs
							int mat_count_spec = 0;
							anim_config.read(reinterpret_cast<char*>(&mat_count_spec), sizeof(int));
							int mat_count_norm = 0;
							anim_config.read(reinterpret_cast<char*>(&mat_count_norm), sizeof(int));
							int mat_count_emm = 0;
							anim_config.read(reinterpret_cast<char*>(&mat_count_emm), sizeof(int));
						}

						//Update offset
						resourceDescriptorOffset += m_modelNormal.size() * 4; //materials * 4 (max texture count) - THIS IS A BIG OL HACKY FIX, a way to count actual resources would be nice :)

						for (int i = 0; i < number_of_materials; i++) {
							//Get this material's index, and fetch our config object
							int material_index;
							anim_config.read(reinterpret_cast<char*>(&material_index), sizeof(int));
							MaterialConfig& this_mat = m_material_config.at(material_index);
							this_mat.is_animated = true;

							//Get the number of textures in this animation
							int num_of_textures;
							anim_config.read(reinterpret_cast<char*>(&num_of_textures), sizeof(int));

							//Get the animation time
							anim_config.read(reinterpret_cast<char*>(&this_mat.animation_time), sizeof(float));

							//Get each texture to animate
							for (int x = 0; x < num_of_textures; x++) {
								//Fetch texture name
								int this_tex_len;
								anim_config.read(reinterpret_cast<char*>(&this_tex_len), sizeof(int));
								char* this_tex_char = new char[this_tex_len];
								anim_config.read(this_tex_char, this_tex_len);
								std::string this_tex;
								this_tex.append(this_tex_char, this_tex_len);
								this_mat.texture_names.push_back(this_tex);

								//Convert texture name to wstring
								std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
								this_tex = dirpath + this_tex;
								std::wstring this_tex_wstring = converter.from_bytes(this_tex.c_str());

								//Load actual texture by name & store its GPU index
								Microsoft::WRL::ComPtr<ID3D12Resource> this_tex_d3d12;
								wchar_t this_tex_wchar[_MAX_PATH] = {};
								DX::FindMediaFile(this_tex_wchar, _MAX_PATH, this_tex_wstring.c_str());
								DX::ThrowIfFailed(CreateDDSTextureFromFile(device, resourceUpload, this_tex_wchar, this_tex_d3d12.ReleaseAndGetAddressOf()));
								CreateShaderResourceView(device, this_tex_d3d12.Get(), m_resourceDescriptors->GetCpuHandle(resourceDescriptorOffset));
								this_mat.gpu_indexes.push_back(resourceDescriptorOffset);
								this_mat.d3d12_textures.push_back(this_tex_d3d12);
								resourceDescriptorOffset++;
							}
						}

						DebugText::print("Model '" + filename + "' has " + std::to_string(number_of_materials) + " animated materials.");
					}
					else
					{
						DebugText::print("WARNING: MODEL '" + filename + "' USES A DEPRECIATED ANIMATION CONFIGURATION.");
					}
				}
				else
				{
					DebugText::print("WARNING: MODEL '" + filename + "' DOES NOT HAVE AN ANIMATION CONFIGURATION.");
				}
			}
			#endif
		}

		//Finish upload and wait for it to end
		auto uploadResourcesFinished = resourceUpload.End(Locator::getDD()->m_deviceResources->GetCommandQueue());
		Locator::getDD()->m_deviceResources->WaitForGpu();
		uploadResourcesFinished.wait();
	}
	m_loaded = true;
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
	m_material_config.clear();
	m_loaded = false;
}

/* Update */
void SDKMeshGO3D::Tick()
{
	GameObject3D::Tick();
}
