#pragma once

#include "pch.h"
#include "Camera.h"
#include "RenderTexture.h"

struct ThICC_RenderData {
	std::unique_ptr<DirectX::CommonStates>          m_states;
	std::unique_ptr<DirectX::PBREffectFactory>      m_gameMapPBRFactory; //m_fxFactory

	static const size_t MAX_BACK_BUFFER_COUNT = 3;

	UINT                                                m_backBufferIndex;

	// Direct3D objects.
	Microsoft::WRL::ComPtr<ID3D12Device>                m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[MAX_BACK_BUFFER_COUNT];

	// Swap chain objects.
	Microsoft::WRL::ComPtr<IDXGIFactory4>               m_dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[MAX_BACK_BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D12Resource>              m_depthStencil;

	// Presentation fence objects.
	Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence;
	UINT64                                              m_fenceValues[MAX_BACK_BUFFER_COUNT];
	Microsoft::WRL::Wrappers::Event                     m_fenceEvent;

	// Direct3D rendering objects.
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_rtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
	UINT                                                m_rtvDescriptorSize;
	D3D12_VIEWPORT                                      m_screenViewport;
	D3D12_RECT                                          m_scissorRect;

	// Direct3D properties.
	DXGI_FORMAT                                         m_backBufferFormat;
	DXGI_FORMAT                                         m_depthBufferFormat;
	UINT                                                m_backBufferCount;
	D3D_FEATURE_LEVEL                                   m_d3dMinFeatureLevel;

	// Cached device properties.
	HWND                                                m_window;
	D3D_FEATURE_LEVEL                                   m_d3dFeatureLevel;
	DWORD                                               m_dxgiFactoryFlags;
	RECT                                                m_outputSize;

	// HDR Support
	DXGI_COLOR_SPACE_TYPE                               m_colorSpace;

	// DeviceResources options (see flags above)
	unsigned int                                        m_options;

	int m_window_width;
	int m_window_height;





	/* IMPORTED */

	std::unique_ptr<DirectX::EffectFactory> m_fxFactory = NULL;
	std::unique_ptr<DirectX::SpriteFont> m_font = NULL;
	int m_fontResNum;
	std::unique_ptr<DirectX::DescriptorHeap> m_resourceDescriptors = NULL;
	int m_resourceCount = 0;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch = NULL;
	std::unique_ptr<DirectX::BasicEffect> m_GPeffect = NULL;

	Camera* m_cam = NULL;
	//Light* m_light = NULL;
};