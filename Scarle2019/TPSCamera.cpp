#include "pch.h"
#include "TPSCamera.h"

//TPSCamera::TPSCamera(float _width, float _height, float _near, float _far, GameObject3D* _target, Vector3 _dpos)
//	:Camera(_width, _height, _near, _far)
//{
//	m_targetObject = _target;
//	m_dpos = _dpos;
//}
//
//TPSCamera::~TPSCamera()
//{
//
//}
//
//void TPSCamera::Tick(GameStateData* _GSD)
//{
//	//Set up position of camera and target position of camera based on new position and orientation of target object
//	Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_targetObject->GetYaw(), 0.0f, 0.0f);
//	m_pos = m_targetObject->GetPos() + Vector3::Transform(m_dpos, rotCam);
//
//	//and then set up proj and view matrices
//	Camera::Tick(_GSD);
//}

