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

		phys_data.scale = model_data["modelscale"];
		SetScale(phys_data.scale);
		/*
		For whatever reason, setting this here will cause horrible collision errors...
		SetPos(Vector3(model_data["start_x"], model_data["start_y"], model_data["start_z"]));
		SetRotationInDegrees(Vector3(model_data["rot_x"], model_data["rot_y"], model_data["rot_z"]));
		*/

		if (m_has_collider)
		{
			initCollider(model_data);

			collider_debug = new SDKMeshGO3D(_RD, _filename + " DEBUG");
		}
	}
}

void PhysModel::initCollider(json &model_data)
{
	has_collider = true;

	XMFLOAT3 top_left = XMFLOAT3((float) model_data["collision_box"]["front_top_left"][0] * phys_data.scale,
		(float) model_data["collision_box"]["front_top_left"][1] * phys_data.scale,
		(float) model_data["collision_box"]["front_top_left"][2] * phys_data.scale);

	XMFLOAT3 bottom_right = XMFLOAT3((float) model_data["collision_box"]["back_bottom_right"][0] * phys_data.scale,
		(float) model_data["collision_box"]["back_bottom_right"][1] * phys_data.scale,
		(float) model_data["collision_box"]["back_bottom_right"][2] * phys_data.scale);

	m_coll_local_centre = XMFLOAT3((top_left.x + bottom_right.x) / 2, (top_left.y + bottom_right.y) / 2, (top_left.z + bottom_right.z) / 2);
	m_coll_world_centre = Vector3::Transform(m_coll_local_centre, m_world);

	//Have to split the rotation matrix and reassemble because yaw and pitch are swapped

	XMFLOAT3 euler = MatrixDecomposeYawPitchRoll(m_rot);
	m_collider = BoundingOrientedBox(m_coll_world_centre, top_left, XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.x, euler.y, euler.z)));
}

float PhysModel::getEulerVal(float val)
{
	if (val > M_PI/2)
	{
		return -val;
	}
	/*else if (val < -M_PI)
	{
		return val + M_PI;
	}*/

	return val;
}

XMFLOAT3 PhysModel::MatrixDecomposeYawPitchRoll(Matrix  mat)
{
	XMFLOAT3 euler;
	euler.x = asinf(-mat._32);                  // Pitch
	if (cosf(euler.x) > 0.0001)                 // Not at poles
	{
		euler.y = atan2f(mat._31, mat._33);     // Yaw
		euler.z = atan2f(mat._12, mat._22);     // Roll
	}
	else
	{
		euler.y = 0.0f;                         // Yaw
		euler.z = atan2f(-mat._21, mat._11);    // Roll
	}
	return euler;
}

void PhysModel::updateCollider()
{
	if (m_has_collider)
	{
		m_coll_world_centre = Vector3::Transform(m_coll_local_centre, m_world);
		m_collider.Center = m_coll_world_centre;

		XMFLOAT3 euler = MatrixDecomposeYawPitchRoll(m_rot);
		m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z));

		//Update collider position
		//This might not be entirely accurate to the actual collider position - Evan you may want to adust?

		collider_debug->SetPos(m_coll_world_centre);
		collider_debug->SetScale(m_collider.Extents);	
		Quaternion coll_rot = m_collider.Orientation;

		collider_debug->SetYaw(euler.y);
		collider_debug->SetPitch(euler.x);
		collider_debug->SetRoll(euler.z);
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

		m_pos += _GSD->m_dt * m_velTotal;

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