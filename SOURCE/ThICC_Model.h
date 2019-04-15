#pragma once

#include "pch.h"
#include "AssetFilepaths.h"
#include "ReadData.h"
#include <string>

class ThICC_Model {
public:
	ThICC_Model();
	~ThICC_Model();

	void render();

	void load(std::string _filename);
	void reset();

private:
	AssetFilepaths m_filepath;

	bool loaded = false;

	std::unique_ptr<DirectX::EffectFactory>         m_fxFactory; //SDKMESH1
	std::unique_ptr<DirectX::PBREffectFactory>      m_pbrFXFactory; //SDKMESH2
	std::unique_ptr<DirectX::EffectTextureFactory>  m_modelResources;
	std::unique_ptr<DirectX::Model>                 m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>>  m_modelClockwise;
};