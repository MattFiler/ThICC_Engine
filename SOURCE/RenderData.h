#pragma once

#include "pch.h"
#include "Camera.h"
#include "RenderTexture.h"
#include "Constants.h"

struct ThICC_RenderData {
	//States and FX factory
	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::PBREffectFactory> m_fxFactoryPBR;

	//Back-buffer tracking
	static const size_t MAX_BACK_BUFFER_COUNT = 3;
	UINT m_backBufferIndex;

	// Direct3D objects.
	Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[MAX_BACK_BUFFER_COUNT];

	// Swap chain objects.
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[MAX_BACK_BUFFER_COUNT];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencil;

	// Presentation fence objects.
	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[MAX_BACK_BUFFER_COUNT];
	Microsoft::WRL::Wrappers::Event m_fenceEvent;

	// Direct3D rendering objects.
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvDescriptorHeap;
	UINT m_rtvDescriptorSize;

	//Viewports
	D3D12_VIEWPORT m_screenViewport;
	D3D12_RECT m_scissorRect;
	D3D12_VIEWPORT m_screenViewportSplitscreen[4];
	D3D12_RECT m_scissorRectSplitscreen[4];

	// Direct3D properties.
	DXGI_FORMAT m_backBufferFormat;
	DXGI_FORMAT m_depthBufferFormat;
	UINT m_backBufferCount;
	D3D_FEATURE_LEVEL m_d3dMinFeatureLevel;

	// Cached device properties.
	HWND m_window;
	D3D_FEATURE_LEVEL m_d3dFeatureLevel;
	DWORD m_dxgiFactoryFlags;
	RECT m_outputSize;

	// HDR Support
	DXGI_COLOR_SPACE_TYPE m_colorSpace;

	// DeviceResources options (see flags above)
	unsigned int m_options;

	//Window size (is this correct? what if window resizes, do we update these?)
	int m_window_width;
	int m_window_height;

	//The active camera
	Camera* m_cam = NULL;

	//Shared 2D resources
	std::unique_ptr<DirectX::DescriptorHeap> m_2dResourceDescriptors = NULL;
	std::unique_ptr<DirectX::SpriteBatch> m_2dSpriteBatch = NULL;
	std::unique_ptr<DirectX::SpriteFont> m_2dFont = NULL;
	int m_resourceCount = 0;

	//Current cubemaps (set on map load)
	std::wstring current_cubemap_radiance;
	std::wstring current_cubemap_irradiance;

	//Current metalness config for track
	std::vector<bool> current_metalness;
};