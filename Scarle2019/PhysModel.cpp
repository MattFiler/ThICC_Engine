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
	/*Gets the the top front left and back bottom right points of the mesh from the json file - finds the average between them to get the centre of the mesh
	and uses (currently) the top front left point to determine the extents (size) of the collider */
	has_collider = true;

	m_front_top_left = Vector3((float)model_data["collision_box"]["front_top_left"][0] * phys_data.scale,
		(float)model_data["collision_box"]["front_top_left"][1] * phys_data.scale,
		(float)model_data["collision_box"]["front_top_left"][2] * phys_data.scale);

	m_front_top_right = Vector3((float)model_data["collision_box"]["front_top_right"][0] * phys_data.scale,
		(float)model_data["collision_box"]["front_top_right"][1] * phys_data.scale,
		(float)model_data["collision_box"]["front_top_right"][2] * phys_data.scale);

	m_back_bottom_left = Vector3((float)model_data["collision_box"]["back_bottom_left"][0] * phys_data.scale,
		(float)model_data["collision_box"]["back_bottom_left"][1] * phys_data.scale,
		(float)model_data["collision_box"]["back_bottom_left"][2] * phys_data.scale);

	m_back_bottom_right = Vector3((float)model_data["collision_box"]["back_bottom_right"][0] * phys_data.scale,
		(float)model_data["collision_box"]["back_bottom_right"][1] * phys_data.scale,
		(float)model_data["collision_box"]["back_bottom_right"][2] * phys_data.scale);

	m_coll_local_centre = XMFLOAT3((m_front_top_left.x + m_back_bottom_right.x) / 2, (m_front_top_left.y + m_back_bottom_right.y) / 2, (m_front_top_left.z + m_back_bottom_right.z) / 2);
	//Updates the centre and rotations of the collider 
	m_coll_world_centre = Vector3::Transform(m_coll_local_centre, m_world);
	m_collider.Center = m_coll_world_centre;

	XMFLOAT3 euler = MatrixDecomposeYawPitchRoll(m_rot);
	m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z));

	//Storing the Corners of the collider for Toby
	m_global_front_left = Vector3::Transform(Vector3(m_front_top_left.x, m_coll_local_centre.y, m_front_top_left.z), m_world);
	m_global_front_right = Vector3::Transform(Vector3(m_front_top_right.x, m_coll_local_centre.y, m_front_top_right.z), m_world);
	m_global_back_left = Vector3::Transform(Vector3(m_back_bottom_left.x, m_coll_local_centre.y, m_back_bottom_left.z), m_world);
	m_global_back_right = Vector3::Transform(Vector3(m_back_bottom_right.x, m_coll_local_centre.y, m_back_bottom_right.z), m_world);
	m_width = m_front_top_left.x - m_front_top_right.x;
	m_height = m_front_top_left.y - m_back_bottom_left.y;
	m_length = m_front_top_left.z - m_back_bottom_left.z;
}

XMFLOAT3 PhysModel::MatrixDecomposeYawPitchRoll(Matrix  mat)
{
	//Breaks down a matrix into yaw, pitch, and roll. Returns them as a float3
	XMFLOAT3 euler;
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

void PhysModel::updateCollider()
{
	if (m_has_collider)
	{
		//Updates the centre and rotations of the collider 
		m_coll_world_centre = Vector3::Transform(m_coll_local_centre, m_world);
		m_collider.Center = m_coll_world_centre;

		XMFLOAT3 euler = MatrixDecomposeYawPitchRoll(m_rot);
		m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z));

		//Storing the Corners of the collider for Toby
		m_global_front_left = Vector3::Transform(Vector3(m_front_top_left.x, m_coll_local_centre.y, m_front_top_left.z), m_world);
		m_global_front_right = Vector3::Transform(Vector3(m_front_top_right.x, m_coll_local_centre.y, m_front_top_right.z), m_world);
		m_global_back_left = Vector3::Transform(Vector3(m_back_bottom_left.x, m_coll_local_centre.y, m_back_bottom_left.z), m_world);
		m_global_back_right = Vector3::Transform(Vector3(m_back_bottom_right.x, m_coll_local_centre.y, m_back_bottom_right.z), m_world);

		//Updates the debug collider position and rotation

		collider_debug->SetPos(m_coll_world_centre);
		collider_debug->SetScale(m_collider.Extents);	
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

		//Collision Code
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