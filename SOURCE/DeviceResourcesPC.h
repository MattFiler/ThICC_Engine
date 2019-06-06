//
// DeviceResources.h - A wrapper for the Direct3D 12 device and swapchain
//

#pragma once

#include "RenderData.h"

namespace DX
{
    // Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
    interface IDeviceNotify
    {
        virtual void OnDeviceLost() = 0;
        virtual void OnDeviceRestored() = 0;
    };

    // Controls all the DirectX device resources.
    class DeviceResources
    {
    public:
        static const unsigned int c_AllowTearing    = 0x1;
        static const unsigned int c_EnableHDR       = 0x2;

        DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
                        DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT,
                        UINT backBufferCount = 2,
                        D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0,
                        unsigned int flags = 0) noexcept(false);
        ~DeviceResources();

        void CreateDeviceResources();
        void CreateWindowSizeDependentResources();
        void SetWindow(HWND window, int width, int height);
        bool WindowSizeChanged(int width, int height);
        void HandleDeviceLost();
        void RegisterDeviceNotify(IDeviceNotify* deviceNotify) { m_deviceNotify = deviceNotify; }
        void Prepare(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_PRESENT);
        void Present(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_RENDER_TARGET);
        void WaitForGpu() noexcept;

        // Device Accessors.
        RECT GetOutputSize() const { return m_rd.m_outputSize; }

        // Direct3D Accessors.
        ID3D12Device*               GetD3DDevice() const            { return m_rd.m_d3dDevice.Get(); }
        IDXGISwapChain3*            GetSwapChain() const            { return m_rd.m_swapChain.Get(); }
        D3D_FEATURE_LEVEL           GetDeviceFeatureLevel() const   { return m_rd.m_d3dFeatureLevel; }
        ID3D12Resource*             GetRenderTarget() const         { return m_rd.m_renderTargets[m_rd.m_backBufferIndex].Get(); }
        ID3D12Resource*             GetDepthStencil() const         { return m_rd.m_depthStencil.Get(); }
        ID3D12CommandQueue*         GetCommandQueue() const         { return m_rd.m_commandQueue.Get(); }
        ID3D12CommandAllocator*     GetCommandAllocator() const     { return m_rd.m_commandAllocators[m_rd.m_backBufferIndex].Get(); }
        ID3D12GraphicsCommandList*  GetCommandList() const          { return m_rd.m_commandList.Get(); }
        DXGI_FORMAT                 GetBackBufferFormat() const     { return m_rd.m_backBufferFormat; }
        DXGI_FORMAT                 GetDepthBufferFormat() const    { return m_rd.m_depthBufferFormat; }
        D3D12_VIEWPORT              GetScreenViewport() const       { return m_rd.m_fullscreenViewport; }
        D3D12_RECT                  GetScissorRect() const          { return m_rd.m_fullscreenScissorRect; }
        UINT                        GetCurrentFrameIndex() const    { return m_rd.m_backBufferIndex; }
        UINT                        GetBackBufferCount() const      { return m_rd.m_backBufferCount; }
        DXGI_COLOR_SPACE_TYPE       GetColorSpace() const           { return m_rd.m_colorSpace; }
        unsigned int                GetDeviceOptions() const        { return m_rd.m_options; }
        HWND                        GetWindowHandle() const         { return m_rd.m_window; }

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const
        {
            return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rd.m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_rd.m_backBufferIndex, m_rd.m_rtvDescriptorSize);
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
        {
            return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rd.m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        }

    private:
        void MoveToNextFrame();
        void GetAdapter(IDXGIAdapter1** ppAdapter);
        void UpdateColorSpace();

		ThICC_RenderData m_rd;

		// The IDeviceNotify can be held directly as it owns the DeviceResources.
		DX::IDeviceNotify*                                  m_deviceNotify;
    };
}
