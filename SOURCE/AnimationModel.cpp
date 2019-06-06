#include "pch.h"
#include "AnimationModel.h"


AnimationModel::AnimationModel(SDKMeshGO3D* _model, std::string _name)
{
	m_model = std::unique_ptr<SDKMeshGO3D>(_model);
	m_name = _name;
	m_offset = m_model->GetPos();
	m_originalScale = m_model->GetScale();
	m_currentScale = m_model->GetScale();

	m_originalRotation = m_model->GetRotation();
}


AnimationModel::~AnimationModel()
{
}

void AnimationModel::ResetScale()
{
	m_model->SetScale(m_originalScale);
	m_currentScale = m_originalScale;
}

void AnimationModel::SetWorld(Matrix _newWorld)
{
	m_model->SetWorld(_newWorld);
	m_model->AddPos(Vector3::Transform(m_offset * m_scaleOffset, m_model->GetOri()));
	m_model->AddPos(m_posOffset);
	m_model->SetScale(m_model->GetScale() * m_currentScale);
	m_model->SetRotation(m_model->GetRotation() + m_originalRotation);
	m_model->UpdateWorld();
}