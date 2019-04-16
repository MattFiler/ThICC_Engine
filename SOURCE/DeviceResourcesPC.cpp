﻿//
// DeviceResources.cpp - A wrapper for the Direct3D 12 device and swapchain
//

#include "pch.h"
#include "DeviceResourcesPC.h"
#include "ServiceLocator.h"

using namespace DirectX;
using namespace DX;

using Microsoft::WRL::ComPtr;

namespace
{
    inline DXGI_FORMAT NoSRGB(DXGI_FORMAT fmt)
    {
        switch (fmt)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return DXGI_FORMAT_R8G8B8A8_UNORM;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8A8_UNORM;
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8X8_UNORM;
        default:                                return fmt;
        }
    }
};

// Constructor for DeviceResources.
DeviceResources::DeviceResources(
    DXGI_FORMAT backBufferFormat,
    DXGI_FORMAT depthBufferFormat,
    UINT backBufferCount,
    D3D_FEATURE_LEVEL minFeatureLevel,
    unsigned int flags) noexcept(false)
{
	m_rd.m_backBufferIndex = 0;
	//m_render_data.m_fenceValues = {};
	m_rd.m_rtvDescriptorSize = 0;
	m_rd.m_screenViewport = {};
	m_rd.m_scissorRect = {};
	m_rd.m_backBufferFormat = backBufferFormat;
	m_rd.m_depthBufferFormat = depthBufferFormat;
	m_rd.m_backBufferCount = backBufferCount;
	m_rd.m_d3dMinFeatureLevel = minFeatureLevel;
	m_rd.m_window = nullptr;
	m_rd.m_d3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	m_rd.m_dxgiFactoryFlags = 0;
	m_rd.m_outputSize = { 0, 0, 1, 1 };
	m_rd.m_colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
	m_rd.m_options = flags;
	m_deviceNotify = nullptr;

	Locator::setupRD(&m_rd);

    if (backBufferCount > m_rd.MAX_BACK_BUFFER_COUNT)
    {
        throw std::out_of_range("backBufferCount too large");
    }

    if (minFeatureLevel < D3D_FEATURE_LEVEL_11_0)
    {
        throw std::out_of_range("minFeatureLevel too low");
    }
}

// Destructor for DeviceResources.
DeviceResources::~DeviceResources()
{
    // Ensure that the GPU is no longer referencing resources that are about to be destroyed.
    WaitForGpu();
}

// Configures the Direct3D device, and stores handles to it and the device context.
void DeviceResources::CreateDeviceResources() 
{
#if defined(_DEBUG)
    // Enable the debug layer (requires the Graphics Tools "optional feature").
    //
    // NOTE: Enabling the debug layer after device creation will invalidate the active device.
    {
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
        {
            debugController->EnableDebugLayer();
        }
        else
        {
            OutputDebugStringA("WARNING: Direct3D Debug Device is not available\n");
        }

        ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
        {
            m_rd.m_dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
        }
    }
#endif

    DX::ThrowIfFailed(CreateDXGIFactory2(m_rd.m_dxgiFactoryFlags, IID_PPV_ARGS(m_rd.m_dxgiFactory.ReleaseAndGetAddressOf())));

    // Determines whether tearing support is available for fullscreen borderless windows.
    if (m_rd.m_options & c_AllowTearing)
    {
        BOOL allowTearing = FALSE;

        ComPtr<IDXGIFactory5> factory5;
        HRESULT hr = m_rd.m_dxgiFactory.As(&factory5);
        if (SUCCEEDED(hr))
        {
            hr = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
        }

        if (FAILED(hr) || !allowTearing)
        {
            m_rd.m_options &= ~c_AllowTearing;
#ifdef _DEBUG
            OutputDebugStringA("WARNING: Variable refresh rate displays not supported");
#endif
        }
    }

    ComPtr<IDXGIAdapter1> adapter;
    GetAdapter(adapter.GetAddressOf());

    // Create the DX12 API device object.
    DX::ThrowIfFailed(D3D12CreateDevice(
        adapter.Get(),
        m_rd.m_d3dMinFeatureLevel,
        IID_PPV_ARGS(m_rd.m_d3dDevice.ReleaseAndGetAddressOf())
        ));

    m_rd.m_d3dDevice->SetName(L"DeviceResources");

#ifndef NDEBUG
    // Configure debug device (if active).
    ComPtr<ID3D12InfoQueue> d3dInfoQueue;
    if (SUCCEEDED(m_rd.m_d3dDevice.As(&d3dInfoQueue)))
    {
#ifdef _DEBUG
        d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif
        D3D12_MESSAGE_ID hide[] =
        {
            D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
            D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,
        };
        D3D12_INFO_QUEUE_FILTER filter = {};
        filter.DenyList.NumIDs = _countof(hide);
        filter.DenyList.pIDList = hide;
        d3dInfoQueue->AddStorageFilterEntries(&filter);
    }
#endif

    // Determine maximum supported feature level for this device
    static const D3D_FEATURE_LEVEL s_featureLevels[] =
    {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    D3D12_FEATURE_DATA_FEATURE_LEVELS featLevels =
    {
        _countof(s_featureLevels), s_featureLevels, D3D_FEATURE_LEVEL_11_0
    };

    HRESULT hr = m_rd.m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featLevels, sizeof(featLevels));
    if (SUCCEEDED(hr))
    {
        m_rd.m_d3dFeatureLevel = featLevels.MaxSupportedFeatureLevel;
    }
    else
    {
        m_rd.m_d3dFeatureLevel = m_rd.m_d3dMinFeatureLevel;
    }

    // Create the command queue.
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    DX::ThrowIfFailed(m_rd.m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_rd.m_commandQueue.ReleaseAndGetAddressOf())));

    m_rd.m_commandQueue->SetName(L"DeviceResources");

    // Create descriptor heaps for render target views and depth stencil views.
    D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
    rtvDescriptorHeapDesc.NumDescriptors = m_rd.m_backBufferCount;
    rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    DX::ThrowIfFailed(m_rd.m_d3dDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(m_rd.m_rtvDescriptorHeap.ReleaseAndGetAddressOf())));

    m_rd.m_rtvDescriptorHeap->SetName(L"DeviceResources");

    m_rd.m_rtvDescriptorSize = m_rd.m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    if (m_rd.m_depthBufferFormat != DXGI_FORMAT_UNKNOWN)
    {
        D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeapDesc = {};
        dsvDescriptorHeapDesc.NumDescriptors = 1;
        dsvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

        DX::ThrowIfFailed(m_rd.m_d3dDevice->CreateDescriptorHeap(&dsvDescriptorHeapDesc, IID_PPV_ARGS(m_rd.m_dsvDescriptorHeap.ReleaseAndGetAddressOf())));

        m_rd.m_dsvDescriptorHeap->SetName(L"DeviceResources");
    }

    // Create a command allocator for each back buffer that will be rendered to.
    for (UINT n = 0; n < m_rd.m_backBufferCount; n++)
    {
        DX::ThrowIfFailed(m_rd.m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_rd.m_commandAllocators[n].ReleaseAndGetAddressOf())));

        wchar_t name[25] = {};
        swprintf_s(name, L"Render target %u", n);
        m_rd.m_commandAllocators[n]->SetName(name);
    }

    // Create a command list for recording graphics commands.
    DX::ThrowIfFailed(m_rd.m_d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_rd.m_commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(m_rd.m_commandList.ReleaseAndGetAddressOf())));
    DX::ThrowIfFailed(m_rd.m_commandList->Close());

    m_rd.m_commandList->SetName(L"DeviceResources");

    // Create a fence for tracking GPU execution progress.
    DX::ThrowIfFailed(m_rd.m_d3dDevice->CreateFence(m_rd.m_fenceValues[m_rd.m_backBufferIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_rd.m_fence.ReleaseAndGetAddressOf())));
    m_rd.m_fenceValues[m_rd.m_backBufferIndex]++;

	m_rd.m_fence->SetName(L"DeviceResources");

    m_rd.m_fenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
    if (!m_rd.m_fenceEvent.IsValid())
    {
        throw std::exception("CreateEvent");
    }

	//Fill out our resource descriptor
	Locator::getRD()->m_resourceDescriptors = std::make_unique<DescriptorHeap>(m_rd.m_d3dDevice.Get(),
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		100);
}

// These resources need to be recreated every time the window size is changed.
void DeviceResources::CreateWindowSizeDependentResources() 
{
    if (!m_rd.m_window)
    {
        throw std::exception("Call SetWindow with a valid Win32 window handle");
    }

    // Wait until all previous GPU work is complete.
    WaitForGpu();

    // Release resources that are tied to the swap chain and update fence values.
    for (UINT n = 0; n < m_rd.m_backBufferCount; n++)
    {
        m_rd.m_renderTargets[n].Reset();
        m_rd.m_fenceValues[n] = m_rd.m_fenceValues[m_rd.m_backBufferIndex];
    }

    // Determine the render target size in pixels.
    UINT backBufferWidth = std::max<UINT>(m_rd.m_outputSize.right - m_rd.m_outputSize.left, 1);
    UINT backBufferHeight = std::max<UINT>(m_rd.m_outputSize.bottom - m_rd.m_outputSize.top, 1);
    DXGI_FORMAT backBufferFormat = NoSRGB(m_rd.m_backBufferFormat);

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_rd.m_swapChain)
    {
        // If the swap chain already exists, resize it.
        HRESULT hr = m_rd.m_swapChain->ResizeBuffers(
            m_rd.m_backBufferCount,
            backBufferWidth,
            backBufferHeight,
            backBufferFormat,
            (m_rd.m_options & c_AllowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0
            );

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
#ifdef _DEBUG
            char buff[64] = {};
            sprintf_s(buff, "Device Lost on ResizeBuffers: Reason code 0x%08X\n", (hr == DXGI_ERROR_DEVICE_REMOVED) ? m_rd.m_d3dDevice->GetDeviceRemovedReason() : hr);
            OutputDebugStringA(buff);
#endif
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            HandleDeviceLost();

            // Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = m_rd.m_backBufferCount;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
        swapChainDesc.Flags = (m_rd.m_options & c_AllowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a swap chain for the window.
        ComPtr<IDXGISwapChain1> swapChain;
        DX::ThrowIfFailed(m_rd.m_dxgiFactory->CreateSwapChainForHwnd(
            m_rd.m_commandQueue.Get(),
            m_rd.m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            swapChain.GetAddressOf()
            ));

        DX::ThrowIfFailed(swapChain.As(&m_rd.m_swapChain));

        // This class does not support exclusive full-screen mode and prevents DXGI from responding to the ALT+ENTER shortcut
        DX::ThrowIfFailed(m_rd.m_dxgiFactory->MakeWindowAssociation(m_rd.m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Handle color space settings for HDR
    UpdateColorSpace();

    // Obtain the back buffers for this window which will be the final render targets
    // and create render target views for each of them.
    for (UINT n = 0; n < m_rd.m_backBufferCount; n++)
    {
        DX::ThrowIfFailed(m_rd.m_swapChain->GetBuffer(n, IID_PPV_ARGS(m_rd.m_renderTargets[n].GetAddressOf())));

        wchar_t name[25] = {};
        swprintf_s(name, L"Render target %u", n);
        m_rd.m_renderTargets[n]->SetName(name);

        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = m_rd.m_backBufferFormat;
        rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(m_rd.m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), n, m_rd.m_rtvDescriptorSize);
        m_rd.m_d3dDevice->CreateRenderTargetView(m_rd.m_renderTargets[n].Get(), &rtvDesc, rtvDescriptor);
    }

    // Reset the index to the current back buffer.
    m_rd.m_backBufferIndex = m_rd.m_swapChain->GetCurrentBackBufferIndex();

    if (m_rd.m_depthBufferFormat != DXGI_FORMAT_UNKNOWN)
    {
        // Allocate a 2-D surface as the depth/stencil buffer and create a depth/stencil view
        // on this surface.
        CD3DX12_HEAP_PROPERTIES depthHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

        D3D12_RESOURCE_DESC depthStencilDesc = CD3DX12_RESOURCE_DESC::Tex2D(
            m_rd.m_depthBufferFormat,
            backBufferWidth,
            backBufferHeight,
            1, // This depth stencil view has only one texture.
            1  // Use a single mipmap level.
            );
        depthStencilDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
        depthOptimizedClearValue.Format = m_rd.m_depthBufferFormat;
        depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
        depthOptimizedClearValue.DepthStencil.Stencil = 0;

        DX::ThrowIfFailed(m_rd.m_d3dDevice->CreateCommittedResource(
            &depthHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &depthStencilDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &depthOptimizedClearValue,
            IID_PPV_ARGS(m_rd.m_depthStencil.ReleaseAndGetAddressOf())
            ));

        m_rd.m_depthStencil->SetName(L"Depth stencil");

        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = m_rd.m_depthBufferFormat;
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

        m_rd.m_d3dDevice->CreateDepthStencilView(m_rd.m_depthStencil.Get(), &dsvDesc, m_rd.m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    }

    // Set the 3D rendering viewport and scissor rectangle to target the entire window.
    m_rd.m_screenViewport.TopLeftX = m_rd.m_screenViewport.TopLeftY = 0.f;
    m_rd.m_screenViewport.Width = static_cast<float>(backBufferWidth);
    m_rd.m_screenViewport.Height = static_cast<float>(backBufferHeight);
    m_rd.m_screenViewport.MinDepth = D3D12_MIN_DEPTH;
    m_rd.m_screenViewport.MaxDepth = D3D12_MAX_DEPTH;

    m_rd.m_scissorRect.left = m_rd.m_scissorRect.top = 0;
    m_rd.m_scissorRect.right = backBufferWidth;
    m_rd.m_scissorRect.bottom = backBufferHeight;
}

// This method is called when the Win32 window is created (or re-created).
void DeviceResources::SetWindow(HWND window, int width, int height)
{
    m_rd.m_window = window;

    m_rd.m_outputSize.left = m_rd.m_outputSize.top = 0;
    m_rd.m_outputSize.right = width;
    m_rd.m_outputSize.bottom = height;
}

// This method is called when the Win32 window changes size.
bool DeviceResources::WindowSizeChanged(int width, int height)
{
    RECT newRc;
    newRc.left = newRc.top = 0;
    newRc.right = width;
    newRc.bottom = height;
    if (newRc.left == m_rd.m_outputSize.left
        && newRc.top == m_rd.m_outputSize.top
        && newRc.right == m_rd.m_outputSize.right
        && newRc.bottom == m_rd.m_outputSize.bottom)
    {
        // Handle color space settings for HDR
        UpdateColorSpace();

        return false;
    }

    m_rd.m_outputSize = newRc;
    CreateWindowSizeDependentResources();
    return true;
}

// Recreate all device resources and set them back to the current state.
void DeviceResources::HandleDeviceLost()
{
    if (m_deviceNotify)
    {
        m_deviceNotify->OnDeviceLost();
    }

    for (UINT n = 0; n < m_rd.m_backBufferCount; n++)
    {
        m_rd.m_commandAllocators[n].Reset();
        m_rd.m_renderTargets[n].Reset();
    }

    m_rd.m_depthStencil.Reset();
    m_rd.m_commandQueue.Reset();
    m_rd.m_commandList.Reset();
	m_rd.m_fence.Reset();
    m_rd.m_rtvDescriptorHeap.Reset();
    m_rd.m_dsvDescriptorHeap.Reset();
    m_rd.m_swapChain.Reset();
    m_rd.m_d3dDevice.Reset();
    m_rd.m_dxgiFactory.Reset();

#ifdef _DEBUG
    {
        ComPtr<IDXGIDebug1> dxgiDebug;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
        {
            dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
        }
    }
#endif

    CreateDeviceResources();
    CreateWindowSizeDependentResources();

    if (m_deviceNotify)
    {
        m_deviceNotify->OnDeviceRestored();
    }
}

// Prepare the command list and render target for rendering.
void DeviceResources::Prepare(D3D12_RESOURCE_STATES beforeState)
{
    // Reset command list and allocator.
    DX::ThrowIfFailed(m_rd.m_commandAllocators[m_rd.m_backBufferIndex]->Reset());
    DX::ThrowIfFailed(m_rd.m_commandList->Reset(m_rd.m_commandAllocators[m_rd.m_backBufferIndex].Get(), nullptr));

    if (beforeState != D3D12_RESOURCE_STATE_RENDER_TARGET)
    {
        // Transition the render target into the correct state to allow for drawing into it.
        D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_rd.m_renderTargets[m_rd.m_backBufferIndex].Get(), beforeState, D3D12_RESOURCE_STATE_RENDER_TARGET);
        m_rd.m_commandList->ResourceBarrier(1, &barrier);
    }
}

// Present the contents of the swap chain to the screen.
void DeviceResources::Present(D3D12_RESOURCE_STATES beforeState)
{
    if (beforeState != D3D12_RESOURCE_STATE_PRESENT)
    {
        // Transition the render target to the state that allows it to be presented to the display.
        D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_rd.m_renderTargets[m_rd.m_backBufferIndex].Get(), beforeState, D3D12_RESOURCE_STATE_PRESENT);
        m_rd.m_commandList->ResourceBarrier(1, &barrier);
    }

    // Send the command list off to the GPU for processing.
    DX::ThrowIfFailed(m_rd.m_commandList->Close());
    m_rd.m_commandQueue->ExecuteCommandLists(1, CommandListCast(m_rd.m_commandList.GetAddressOf()));

    HRESULT hr;
    if (m_rd.m_options & c_AllowTearing)
    {
        // Recommended to always use tearing if supported when using a sync interval of 0.
        // Note this will fail if in true 'fullscreen' mode.
        hr = m_rd.m_swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
    }
    else
    {
        // The first argument instructs DXGI to block until VSync, putting the application
        // to sleep until the next VSync. This ensures we don't waste any cycles rendering
        // frames that will never be displayed to the screen.
        hr = m_rd.m_swapChain->Present(1, 0);
    }

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
#ifdef _DEBUG
        char buff[64] = {};
        sprintf_s(buff, "Device Lost on Present: Reason code 0x%08X\n", (hr == DXGI_ERROR_DEVICE_REMOVED) ? m_rd.m_d3dDevice->GetDeviceRemovedReason() : hr);
        OutputDebugStringA(buff);
#endif
        HandleDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);

        MoveToNextFrame();

        if (!m_rd.m_dxgiFactory->IsCurrent())
        {
            // Output information is cached on the DXGI Factory. If it is stale we need to create a new factory.
            DX::ThrowIfFailed(CreateDXGIFactory2(m_rd.m_dxgiFactoryFlags, IID_PPV_ARGS(m_rd.m_dxgiFactory.ReleaseAndGetAddressOf())));
        }
    }
}

// Wait for pending GPU work to complete.
void DeviceResources::WaitForGpu() noexcept
{
    if (m_rd.m_commandQueue && m_rd.m_fence && m_rd.m_fenceEvent.IsValid())
    {
        // Schedule a Signal command in the GPU queue.
        UINT64 fenceValue = m_rd.m_fenceValues[m_rd.m_backBufferIndex];
        if (SUCCEEDED(m_rd.m_commandQueue->Signal(m_rd.m_fence.Get(), fenceValue)))
        {
            // Wait until the Signal has been processed.
            if (SUCCEEDED(m_rd.m_fence->SetEventOnCompletion(fenceValue, m_rd.m_fenceEvent.Get())))
            {
                WaitForSingleObjectEx(m_rd.m_fenceEvent.Get(), INFINITE, FALSE);

                // Increment the fence value for the current frame.
                m_rd.m_fenceValues[m_rd.m_backBufferIndex]++;
            }
        }
    }
}

// Prepare to render the next frame.
void DeviceResources::MoveToNextFrame()
{
    // Schedule a Signal command in the queue.
    const UINT64 currentFenceValue = m_rd.m_fenceValues[m_rd.m_backBufferIndex];
    DX::ThrowIfFailed(m_rd.m_commandQueue->Signal(m_rd.m_fence.Get(), currentFenceValue));

    // Update the back buffer index.
    m_rd.m_backBufferIndex = m_rd.m_swapChain->GetCurrentBackBufferIndex();

    // If the next frame is not ready to be rendered yet, wait until it is ready.
    if (m_rd.m_fence->GetCompletedValue() < m_rd.m_fenceValues[m_rd.m_backBufferIndex])
    {
        DX::ThrowIfFailed(m_rd.m_fence->SetEventOnCompletion(m_rd.m_fenceValues[m_rd.m_backBufferIndex], m_rd.m_fenceEvent.Get()));
        WaitForSingleObjectEx(m_rd.m_fenceEvent.Get(), INFINITE, FALSE);
    }

    // Set the fence value for the next frame.
    m_rd.m_fenceValues[m_rd.m_backBufferIndex] = currentFenceValue + 1;
}

// This method acquires the first available hardware adapter that supports Direct3D 12.
// If no such adapter can be found, try WARP. Otherwise throw an exception.
void DeviceResources::GetAdapter(IDXGIAdapter1** ppAdapter)
{
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;

#if defined(__dxgi1_6_h__) && defined(NTDDI_WIN10_RS4)
    ComPtr<IDXGIFactory6> factory6;
    HRESULT hr = m_rd.m_dxgiFactory.As(&factory6);
    if (SUCCEEDED(hr))
    {
        for (UINT adapterIndex = 0;
            DXGI_ERROR_NOT_FOUND != factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()));
            adapterIndex++)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                continue;
            }

            // Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), m_rd.m_d3dMinFeatureLevel, _uuidof(ID3D12Device), nullptr)))
            {
            #ifdef _DEBUG
                wchar_t buff[256] = {};
                swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
                OutputDebugStringW(buff);
            #endif
                break;
            }
        }
    }
    else
#endif
    for (UINT adapterIndex = 0;
        DXGI_ERROR_NOT_FOUND != m_rd.m_dxgiFactory->EnumAdapters1(
            adapterIndex,
            adapter.ReleaseAndGetAddressOf());
        ++adapterIndex)
    {
        DXGI_ADAPTER_DESC1 desc;
        DX::ThrowIfFailed(adapter->GetDesc1(&desc));

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            // Don't select the Basic Render Driver adapter.
            continue;
        }

        // Check to see if the adapter supports Direct3D 12, but don't create the actual device yet.
        if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), m_rd.m_d3dMinFeatureLevel, _uuidof(ID3D12Device), nullptr)))
        {
        #ifdef _DEBUG
            wchar_t buff[256] = {};
            swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
            OutputDebugStringW(buff);
        #endif
            break;
        }
    }

#if !defined(NDEBUG)
    if (!adapter)
    {
        // Try WARP12 instead
        if (FAILED(m_rd.m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))))
        {
            throw std::exception("WARP12 not available. Enable the 'Graphics Tools' optional feature");
        }

        OutputDebugStringA("Direct3D Adapter - WARP12\n");
    }
#endif

    if (!adapter)
    {
        throw std::exception("No Direct3D 12 device found");
    }

    *ppAdapter = adapter.Detach();
}

// Sets the color space for the swap chain in order to handle HDR output.
void DeviceResources::UpdateColorSpace()
{
    DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

    bool isDisplayHDR10 = false;

#if defined(NTDDI_WIN10_RS2)
    if (m_rd.m_swapChain)
    {
        ComPtr<IDXGIOutput> output;
        if (SUCCEEDED(m_rd.m_swapChain->GetContainingOutput(output.GetAddressOf())))
        {
            ComPtr<IDXGIOutput6> output6;
            if (SUCCEEDED(output.As(&output6)))
            {
                DXGI_OUTPUT_DESC1 desc;
                DX::ThrowIfFailed(output6->GetDesc1(&desc));

                if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
                {
                    // Display output is HDR10.
                    isDisplayHDR10 = true;
                }
            }
        }
    }
#endif

    if ((m_rd.m_options & c_EnableHDR) && isDisplayHDR10)
    {
        switch (m_rd.m_backBufferFormat)
        {
        case DXGI_FORMAT_R10G10B10A2_UNORM:
            // The application creates the HDR10 signal.
            colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
            break;

        case DXGI_FORMAT_R16G16B16A16_FLOAT:
            // The system creates the HDR10 signal; application uses linear values.
            colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
            break;

        default:
            break;
        }
    }

    m_rd.m_colorSpace = colorSpace;

    ComPtr<IDXGISwapChain3> swapChain3;
    if (SUCCEEDED(m_rd.m_swapChain.As(&swapChain3)))
    {
        UINT colorSpaceSupport = 0;
        if (SUCCEEDED(swapChain3->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport))
            && (colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
        {
            DX::ThrowIfFailed(swapChain3->SetColorSpace1(colorSpace));
        }
    }
}
