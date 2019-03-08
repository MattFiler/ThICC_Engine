#pragma once
#include "pch.h"

using namespace DirectX;

struct WindowData
{
	HWND                                                m_window;
	int                                                 m_outputWidth;
	int                                                 m_outputHeight;

	int													m_width;
	int													m_height;

	D3D12_VIEWPORT										m_viewport[4];
	D3D12_RECT											m_scissorRect[4];
	D3D12_VIEWPORT										men_viewport;
	D3D12_RECT											men_scissorRect;

	//// Direct3D Objects
	//D3D_FEATURE_LEVEL                                   m_featureLevel;
	//UINT                                                m_backBufferIndex;
	//UINT                                                m_rtvDescriptorSize;
	//Microsoft::WRL::ComPtr<ID3D12Device>                m_d3dDevice;
	//Microsoft::WRL::ComPtr<IDXGIFactory4>               m_dxgiFactory;
	//Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_commandQueue;
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_rtvDescriptorHeap;
	//Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
	//static const UINT									c_swapBufferCount = 2;
	//Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[c_swapBufferCount];
	//Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
	//Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence;
	//UINT64                                              m_fenceValues[c_swapBufferCount];
	//Microsoft::WRL::Wrappers::Event                     m_fenceEvent;

	//Microsoft::WRL::ComPtr<IDXGISwapChain3>             m_swapChain;
	//Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[c_swapBufferCount];
	//Microsoft::WRL::ComPtr<ID3D12Resource>              m_depthStencil;
};
