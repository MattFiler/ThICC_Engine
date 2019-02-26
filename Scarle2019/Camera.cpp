#include "pch.h"
#include "Camera.h"


Camera::Camera(float _width, float _height, float _near, float _far)
{
	m_pos = Vector3::Backward;
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, _width / _height, _near, _far);
}


Camera::~Camera()
{
}

void Camera::Tick(GameStateData* _GSD)
{

	if (m_targetObject)
	{
		m_view = Matrix::CreateLookAt(m_pos, m_targetObject->GetPos(), Vector3::Up);
	}
	else
	{
		m_view = Matrix::CreateLookAt(m_pos, m_targetPos, Vector3::Up);
	}

	GameObject3D::Tick(_GSD);
}
