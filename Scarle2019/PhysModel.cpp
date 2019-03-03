#include "pch.h"
#include "PhysModel.h"
#include "GameStateData.h"

PhysModel::PhysModel(RenderData * _RD, string _filename) :SDKMeshGO3D(_RD, _filename)
{
}

PhysModel::~PhysModel()
{
}

void PhysModel::Tick(GameStateData * _GSD)
{

	if (m_physicsOn)
	{

		m_vel = m_vel + _GSD->m_dt * (m_acc - m_drag*m_vel);

		m_gravVel = m_gravVel + _GSD->m_dt * (m_gravDirection);
		if (m_gravVel.Length() > m_maxGrav)
		{
			m_gravVel.Normalize();
			m_gravVel *= m_maxGrav;
		}

		m_velTotal = m_vel + m_gravVel;

		m_pos = m_pos + (_GSD->m_dt * m_velTotal);
	}

	SDKMeshGO3D::Tick(_GSD);

	m_acc = Vector3::Zero;
}
