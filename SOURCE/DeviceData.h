#pragma once

#include "pch.h"
#include "DeviceResourcesPC.h"

struct ThICC_DeviceData {
	std::unique_ptr<DX::DeviceResources>            m_deviceResources;

	std::unique_ptr<DX::RenderTexture>              m_hdrScene;
};