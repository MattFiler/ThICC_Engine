#include "pch.h"
#include "Skybox.h"
#include "RenderData.h"
#include "DeviceData.h"
#include "FindMedia.h"
#include <codecvt>

Skybox::~Skybox() {
	Reset();
}

/* Load skybox */
void Skybox::Load() {
	//Our D3D device
	auto device = Locator::getDD()->m_deviceResources->GetD3DDevice();

	//Create our resource descriptor
	skybox_resources = std::make_unique<DescriptorPile>(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 1);

	//Begin the resource upload
	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();

	//Create skybox
	skybox = GeometricPrimitive::CreateSphere(100.0f, 16U, false, true);
	//skybox->LoadStaticBuffers(Locator::getRD()->m_d3dDevice, resourceUpload);

	//Find and load our skybox material
	wchar_t radiance[_MAX_PATH] = {};
	DX::FindMediaFile(radiance, _MAX_PATH, Locator::getRD()->current_cubemap_skybox.c_str());
	DX::ThrowIfFailed(CreateDDSTextureFromFile(device, resourceUpload, radiance, skybox_texture.ReleaseAndGetAddressOf()));
	CreateShaderResourceView(device, skybox_texture.Get(), skybox_resources->GetCpuHandle(0));

	//Create skybox effect and apply material
	RenderTargetState hdrState(Locator::getDD()->m_hdrScene->GetFormat(), Locator::getDD()->m_deviceResources->GetDepthBufferFormat());
	EffectPipelineStateDescription psd(
		&GeometricPrimitive::VertexType::InputLayout,
		CommonStates::Opaque,
		CommonStates::DepthNone,
		CommonStates::CullCounterClockwise,
		hdrState);
	skybox_effect = std::make_unique<BasicEffect>(device, EffectFlags::Texture, psd);
	skybox_effect->SetTexture(skybox_resources->GetGpuHandle(0), Locator::getRD()->m_states->LinearWrap());

	//Finish upload
	auto uploadResourcesFinished = resourceUpload.End(Locator::getDD()->m_deviceResources->GetCommandQueue());
	uploadResourcesFinished.wait();

	is_loaded = true;
}

/* Update skybox transforms */
void Skybox::Tick(Camera* cam) {
	m_pos = cam->GetPos();
	UpdateWorld();

	skybox_effect->SetProjection(cam->GetProj());
	skybox_effect->SetView(cam->GetView());
	skybox_effect->SetWorld(m_world);
}

/* Render skybox */
void Skybox::Render() {
	ID3D12DescriptorHeap* heaps[] = { skybox_resources->Heap(), Locator::getRD()->m_states->Heap() };
	Locator::getDD()->m_deviceResources->GetCommandList()->SetDescriptorHeaps(_countof(heaps), heaps);

	skybox_effect->Apply(Locator::getRD()->m_commandList.Get());
	skybox->Draw(Locator::getRD()->m_commandList.Get());
}

/* Reset skybox */
void Skybox::Reset() {
	skybox.reset();
	skybox_texture.Reset();
	skybox_resources.reset();
	is_loaded = false;
}