#include "pch.h"
#include "GameObject3D.h"

GameObject3D::GameObject3D()
{
}


GameObject3D::~GameObject3D()
{
	Matrix trans = Matrix::CreateTranslation(m_pos);
	Matrix scale = Matrix::CreateScale(m_scale);
	m_rot = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
	m_world = scale * m_rot * trans;
}

void GameObject3D::Tick(GameStateData* _GSD)
{
	Matrix trans = Matrix::CreateTranslation(m_pos);
	Matrix scale = Matrix::CreateScale(m_scale);

	if (m_autoCalculateWolrd)
	{
		m_rot = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
		m_world = scale * m_rot * trans;
	}
}
