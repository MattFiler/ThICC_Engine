#include "pch.h"
#include "PhysModel.h"
#include "GameStateData.h"
#include <iostream>
#include <fstream>

PhysModel::PhysModel(RenderData * _RD, string _filename) :SDKMeshGO3D(_RD, _filename)
{
	std::string test = m_filepath.generateConfigFilepath(_filename, m_filepath.MODEL);
	std::ifstream i(test);

	if (i)
	{
		json model_data;
		model_data << i;
		m_has_collider = model_data["has_box_collider"];
		if (m_has_collider)
		{
			initCollider(model_data);
		}
	}
}

void PhysModel::initCollider(json &model_data)
{
	has_collider = true;

	XMFLOAT3 top_left = XMFLOAT3(model_data["collision_box"]["front_top_left"][0],
		model_data["collision_box"]["front_top_left"][1],
		model_data["collision_box"]["front_top_left"][2]);

	XMFLOAT3 bottom_right = XMFLOAT3(model_data["collision_box"]["back_bottom_right"][0],
		model_data["collision_box"]["back_bottom_right"][1],
		model_data["collision_box"]["back_bottom_right"][2]);

	m_local_centre = XMFLOAT3((top_left.x + bottom_right.x) / 2, (top_left.y + bottom_right.y) / 2, (top_left.z + bottom_right.z) / 2);
	m_world_centre = Vector3::Transform(m_local_centre, m_world);

	m_collider = BoundingOrientedBox(m_world_centre, top_left, XMFLOAT4(Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll)));
}

void PhysModel::updateCollider()
{
	if (m_has_collider)
	{
		m_world_centre = Vector3::Transform(m_local_centre, m_world);
		m_collider.Center = m_world_centre;
		m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(m_yaw, m_pitch, m_roll));
	}
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

		if (m_collided)
		{
			Vector3 normalised_vel = m_vel;
			normalised_vel.Normalize();
			m_pos -= normalised_vel;
			m_vel = Vector3::Zero;
			m_collided = false;
		}
	}

	SDKMeshGO3D::Tick(_GSD);

	m_acc = Vector3::Zero;

	updateCollider();
}