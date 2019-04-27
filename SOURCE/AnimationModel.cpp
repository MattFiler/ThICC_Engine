#include "pch.h"
#include "AnimationModel.h"


AnimationModel::AnimationModel(SDKMeshGO3D* _model, std::string _name, Vector3 _offset)
{
	m_model = std::unique_ptr<SDKMeshGO3D>(_model);
	m_name = _name;
	m_offset = _offset;
}


AnimationModel::~AnimationModel()
{
}
