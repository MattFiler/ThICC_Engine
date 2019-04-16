#include "pch.h"
#include "GameObject3D.h"


GameObject3D::GameObject3D()
{
}


GameObject3D::~GameObject3D()
{
}

void GameObject3D::Tick(GameStateData* _GSD)
{
	Matrix trans = Matrix::CreateTranslation(m_pos);
	m_rot = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
	Matrix scale = Matrix::CreateScale(m_scale);

	m_world = scale * m_rot * trans;
}
