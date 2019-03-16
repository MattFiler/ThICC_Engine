#pragma once
#include "pch.h"

using namespace DirectX;

//GEP:: Various low level objects required by the various rendering systems
struct RenderData
{
	//SDKMeshGO3D renderer
	std::unique_ptr<DirectX::CommonStates> m_states = NULL;
	std::unique_ptr<DirectX::EffectFactory> m_fxFactory = NULL;

	Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice = NULL;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue = NULL;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList = NULL;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[2] = { NULL,NULL };

	//memory fence access
	Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence = NULL;
	UINT64*                                              m_fenceValues = NULL;
	UINT*                                                m_backBufferIndex;

	std::unique_ptr<DirectX::SpriteFont> m_font = NULL;
	int m_fontResNum;

	std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors = NULL;
	int m_resourceCount = 0;

	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch = NULL;

	std::unique_ptr<DirectX::BasicEffect> m_GPeffect = NULL;
	std::unique_ptr<NormalMapEffect> effect = NULL;

	Camera* m_cam = NULL;
	Light* m_light=NULL;

	~RenderData()
	{
		m_states.reset();
		m_fxFactory.reset();
		m_font.reset();
		m_resourceDescriptors.reset();
		m_spriteBatch.reset();
		m_GPeffect.reset();
	}
};