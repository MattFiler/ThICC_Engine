#include "pch.h"
#include "AnimationModel.h"


AnimationModel::AnimationModel(SDKMeshGO3D* _model, std::string _name, Vector3 _offset)
{
	m_model = std::unique_ptr<SDKMeshGO3D>(_model);
	m_name = _name;
	m_offset = _offset;
	m_originalScale = m_model->GetScale();
}


AnimationModel::~AnimationModel()
{
}

void AnimationModel::SetScaleOffset(Vector3 _scaleOffset)
{
	m_scaleOffset = _scaleOffset;
	m_model->SetScale(m_scaleOffset * m_originalScale);
	m_model->UpdateWorld();
}

Vector3 AnimationModel::GetOffset()
{
	return m_offset * m_scaleOffset;
}