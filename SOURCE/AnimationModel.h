#pragma once
#include "SDKMeshGO3D.h";

class AnimationModel
{
public:
	AnimationModel(SDKMeshGO3D* _model, std::string _name);
	~AnimationModel();

	std::string GetName() { return m_name; };
	SDKMeshGO3D* GetModel() { return m_model.get(); };

	Vector3 GetCurrentScale() { return m_currentScale; }
	Vector3 GetPosOffset() { return m_posOffset; }
	void SetCurrentScale(Vector3 _scale) { m_currentScale = _scale; }
	void SetPosOffset(Vector3 _offset) { m_posOffset = _offset; }
	void ResetScale();

	void SetWorld(Matrix _newWorld);

private:
	Vector3 m_originalScale = Vector3::One;
	Vector3 m_currentScale = Vector3::One;
	Vector3 m_scaleOffset = Vector3::One;
	Vector3 m_posOffset = Vector3::Zero;
	Vector3 m_originalRotation = Vector3::Zero;
	Vector3 m_rotationOffset = Vector3::Zero;
	std::unique_ptr<SDKMeshGO3D> m_model = nullptr;
	std::string m_name;
    DirectX::SimpleMath::Vector3 m_offset = Vector3::Zero;
};

