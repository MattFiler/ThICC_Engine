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
	switch (behav)
	{
	case BEHAVIOUR::NORMAL:

		if (perp == PERSPECTIVE::THIRD)
		{
			if (m_targetObject)
			{
				m_view = Matrix::CreateLookAt(m_pos, m_targetObject->GetPos(), Vector3::Up);

				if (m_targetObject->GetUseMagnetMatrix())
				{
					//Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_targetObject->GetYaw(), m_targetObject->GetPitch(), m_targetObject->GetRoll());
					Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_targetObject->GetYaw(), m_targetObject->GetPitch(), m_targetObject->GetRoll());
					Matrix w = m_targetObject->GetWorldMagnet() * m_targetObject->GetWorldMagnet();
					m_pos = m_targetObject->GetPos() * w.Forward(); +Vector3::Transform(m_dpos * w.Forward(), rotCam * w);
				}
				else if (!m_targetObject->GetUseMagnetMatrix())
				{
					Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_targetObject->GetYaw(), m_targetObject->GetPitch(), m_targetObject->GetRoll());
					m_pos = m_targetObject->GetPos() + Vector3::Transform(m_dpos, rotCam);
				}


				//m_pos = m_targetObject->GetPos() + Vector3::Transform(m_dpos, rotCam);
				//if (m_pos != m_targetObject->GetPos() + Vector3::Transform(m_dpos, rotCam))
				//{
				//	m_pos = m_pos + 0.1 * (m_targetObject->GetPos() + Vector3::Transform(m_dpos, rotCam) - m_pos);
				//	m_pos = Vector3::Lerp(m_pos, m_targetObject->GetPos() + Vector3::Transform(m_dpos, rotCam), 0.1);
				//}
			}
			else
			{
				m_view = Matrix::CreateLookAt(m_pos, m_targetPos, Vector3::Up);
			}
		}
		break;
	case BEHAVIOUR::ORBIT:
		m_view = Matrix::CreateLookAt(m_pos, m_targetObject->GetPos(), Vector3::Up);

		Matrix rotCam = Matrix::CreateFromYawPitchRoll(m_targetObject->GetYaw(), m_targetObject->GetPitch(), m_targetObject->GetRoll());

		Vector3 base_pos = m_targetObject->GetPos() + Vector3::Transform(m_dpos, rotCam);
		angle -= 0.4f;
		Vector3 orbit_pos;
		orbit_pos.x = sin(angle / 57.2958f) * (base_pos.x - m_targetObject->GetPos().x);

		orbit_pos.z = cos(angle / 57.2958f) * (base_pos.z - m_targetObject->GetPos().z);
		
		m_pos = m_targetObject->GetPos() + Vector3::Transform({ orbit_pos.x , m_dpos.y, orbit_pos.z }, rotCam);

		break;
	}

	GameObject3D::Tick(_GSD);
}