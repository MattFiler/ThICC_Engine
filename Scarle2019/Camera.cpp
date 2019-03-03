#include "pch.h"
#include "Camera.h"


Camera::Camera(float _width, float _height, float _near, float _far, GameObject3D* _target, Vector3 _dpos)
{
	m_pos = Vector3::Backward;
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, _width / _height, _near, _far);
	m_targetObject = _target;
	m_dpos = _dpos;
}


Camera::~Camera()
{
}

void Camera::Tick(GameStateData* _GSD)
{
	if (m_targetObject)
	{
		m_view = Matrix::CreateLookAt(m_pos, m_targetObject->GetPos(), Vector3::Up);
		//Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_targetObject->GetYaw(), m_targetObject->GetPitch(), m_targetObject->GetRoll());
		Matrix rotCam = m_targetObject->GetOri();
		m_pos = m_targetObject->GetPos() + Vector3::Transform(m_dpos, rotCam);
	}
	else
	{
		m_view = Matrix::CreateLookAt(m_pos, m_targetPos, Vector3::Up);
	}

	GameObject3D::Tick(_GSD);
}
