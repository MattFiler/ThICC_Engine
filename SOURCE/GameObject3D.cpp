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

void GameObject3D::SetRotation(Vector3 _rotation)
{ 
	m_pitch = _rotation.x; 
	m_yaw = _rotation.y; 
	m_roll = _rotation.z; 
	m_rot = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
}

void GameObject3D::SetRotationInDegrees(Vector3 _rotation)
{ 
	m_pitch = _rotation.x / 57.2958; 
	m_yaw = _rotation.y / 57.2958; 
	m_roll = _rotation.z / 57.2958; 
	m_rot = Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll);
}

Vector3 GameObject3D::GetRotation()
{
	return MatrixDecomposeYawPitchRoll(m_rot);
}

Vector3 GameObject3D::MatrixDecomposeYawPitchRoll(Matrix  mat)
{
	//Breaks down a matrix into yaw, pitch, and roll. Returns them as a float3
	Vector3 euler;
	euler.x = asinf(-mat._32);
	if (cosf(euler.x) > 0.0001)
	{
		euler.y = atan2f(mat._31, mat._33);
		euler.z = atan2f(mat._12, mat._22);
	}
	else
	{
		euler.y = 0.0f;
		euler.z = atan2f(-mat._21, mat._11);
	}
	return euler;
}
