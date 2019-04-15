#pragma once

#include "pch.h"

struct ThICC_ModelResources {
	std::unique_ptr<DirectX::EffectFactory>         m_fxFactory; //SDKMESH1
	std::unique_ptr<DirectX::PBREffectFactory>      m_pbrFXFactory; //SDKMESH2
	std::unique_ptr<DirectX::EffectTextureFactory>  m_modelResources;
	std::unique_ptr<DirectX::Model>                 m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>>  m_modelClockwise;
};