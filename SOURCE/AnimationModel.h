#pragma once
#include "SDKMeshGO3D.h";

class AnimationModel
{
public:
	AnimationModel(SDKMeshGO3D* _model, std::string _name, Vector3 _offset);
	~AnimationModel();

	std::string getName() { return m_name; };
	SDKMeshGO3D* getModel() { return m_model.get(); };
	Vector3 getOffset() { return m_offset; };
private:
	std::unique_ptr<SDKMeshGO3D> m_model = nullptr;
	std::string m_name;
    DirectX::SimpleMath::Vector3 m_offset = Vector3::Zero;
};

