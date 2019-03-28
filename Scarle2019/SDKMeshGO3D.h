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
	SDKMeshGO3D(string _filename);
	virtual ~SDKMeshGO3D();

	virtual void Render();

	virtual void Tick() override;

	bool isDebugMesh() { return is_debug_mesh; }

	void Reset() override;

	void SetShouldRender(bool _shouldRender) { m_shouldRender = _shouldRender; };

protected:
	std::unique_ptr<DirectX::EffectTextureFactory> m_modelResources;
	std::unique_ptr<DirectX::Model> m_model;
	std::vector<std::shared_ptr<DirectX::IEffect>> m_modelNormal; 

	GameFilepaths m_filepath;

	bool is_debug_mesh = false;
	bool m_shouldRender = true;
};

