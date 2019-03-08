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

};
