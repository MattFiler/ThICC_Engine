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
		Vector3 newVel = m_vel + _GSD->m_dt * (m_acc - m_drag*m_vel);
		Vector3 newPos = m_pos + _GSD->m_dt * m_vel;

		m_vel = newVel;
		m_pos = newPos;
	}

	SDKMeshGO3D::Tick(_GSD);

	m_acc = Vector3::Zero;
}
