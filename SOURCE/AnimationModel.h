#pragma once
#include "SDKMeshGO3D.h";

class AnimationModel
{
public:
	AnimationModel(SDKMeshGO3D* _model, std::string _name, Vector3 _offset);
	~AnimationModel();

	std::string GetName() { return m_name; };
	SDKMeshGO3D* GetModel() { return m_model.get(); };
	Vector3 GetOffset();

	void SetScaleOffset(Vector3 _scaleOffset);
private:
	Vector3 m_originalScale = Vector3::One;
	Vector3 m_scaleOffset = Vector3::One;
	std::unique_ptr<SDKMeshGO3D> m_model = nullptr;
	std::string m_name;
    DirectX::SimpleMath::Vector3 m_offset = Vector3::Zero;
};

