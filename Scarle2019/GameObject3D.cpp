#include "pch.h"
#include "GameObject3D.h"

GameObject3D::GameObject3D()
{
	Matrix trans = Matrix::CreateTranslation(m_pos);
	Matrix scale = Matrix::CreateScale(m_scale);
	m_rot = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
	m_world = scale * m_rot * trans;
}


GameObject3D::~GameObject3D()
{
}

void GameObject3D::Tick()
{
	Matrix trans = Matrix::CreateTranslation(m_pos);
	Matrix scale = Matrix::CreateScale(m_scale);

	if (m_autoCalculateWolrd)
	{
		m_rot = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
		m_world = scale * m_rot * trans;
	}

	//m_rot = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
	//m_world = scale * m_rot * trans;
}

void GameObject3D::SetWorld(Matrix _world)
{
	m_world = _world;
	m_world.Decompose(m_scale, m_quatRot, m_pos);
	m_rot = Matrix::CreateFromQuaternion(m_quatRot);
}

void GameObject3D::UpdateWorld()
{
	Matrix trans = Matrix::CreateTranslation(m_pos);
	Matrix scale = Matrix::CreateScale(m_scale);
	m_world = scale * m_rot * trans;
}