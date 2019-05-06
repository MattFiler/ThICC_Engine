#pragma once
#include "SDKMeshGO3D.h";

class AnimationModel
{
public:
	AnimationModel(SDKMeshGO3D* _model, std::string _name);
	~AnimationModel();

	std::string GetName() { return m_name; };
	SDKMeshGO3D* GetModel() { return m_model.get(); };

	void SetWorld(Matrix _newWorld);

private:
	Vector3 m_originalScale = Vector3::One;
	Vector3 m_scaleOffset = Vector3::One;
	Vector3 m_originalRotation = Vector3::Zero;
	Vector3 m_rotationOffset = Vector3::Zero;
	std::unique_ptr<SDKMeshGO3D> m_model = nullptr;
	std::string m_name;
    DirectX::SimpleMath::Vector3 m_offset = Vector3::Zero;
};

