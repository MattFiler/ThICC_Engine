#pragma once
#include "GameObject3D.h"
#include "GameFilepaths.h"
#include <string>
struct RenderData;

//GEP:: Uses the SDKMesh format to load in models.
// A basic pipeline is supplied in the Asset Pipeline >> Models Project
// Built using the tutorial here : https://github.com/Microsoft/DirectXTK12/wiki/Rendering-a-model

class SDKMeshGO3D :
	public GameObject3D
{
public:
	SDKMeshGO3D(RenderData* _RD, string _filename);
	virtual ~SDKMeshGO3D();

	virtual void Render(RenderData * _RD);

	virtual void Tick(GameStateData* _GSD) override;

	void Reset() override;

protected:
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<DirectX::Model> m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>> m_modelNormal;

	GameFilepaths m_filepath;
};

