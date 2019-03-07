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
		m_hasCollider = model_data["has_box_collider"];

		m_physData.scale = model_data["modelscale"];
		SetScale(m_physData.scale);
		/*
		For whatever reason, setting this here will cause horrible collision errors...
		SetPos(Vector3(model_data["start_x"], model_data["start_y"], model_data["start_z"]));
		SetRotationInDegrees(Vector3(model_data["rot_x"], model_data["rot_y"], model_data["rot_z"]));
		*/

		if (m_hasCollider)
		{
			initCollider(model_data);

			m_colliderDebug = new SDKMeshGO3D(_RD, _filename + " DEBUG");
		}
	}
}

void PhysModel::initCollider(json &model_data)
{
	/*Gets the the top front left and back bottom right points of the mesh from the json file - finds the average between them to get the centre of the mesh
	and uses (currently) the top front left point to determine the extents (size) of the collider */
	m_hasCollider = true;

	m_frontTopLeft = Vector3((float)model_data["collision_box"]["front_top_left"][0] * m_physData.scale,
		(float)model_data["collision_box"]["front_top_left"][1] * m_physData.scale,
		(float)model_data["collision_box"]["front_top_left"][2] * m_physData.scale);

	m_frontTopRight = Vector3((float)model_data["collision_box"]["front_top_right"][0] * m_physData.scale,
		(float)model_data["collision_box"]["front_top_right"][1] * m_physData.scale,
		(float)model_data["collision_box"]["front_top_right"][2] * m_physData.scale);

	m_backBottomLeft = Vector3((float)model_data["collision_box"]["back_bottom_left"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_left"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_left"][2] * m_physData.scale);

	m_backBottomRight = Vector3((float)model_data["collision_box"]["back_bottom_right"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_right"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_right"][2] * m_physData.scale);

	m_collLocalCentre = XMFLOAT3((m_frontTopLeft.x + m_backBottomRight.x) / 2, (m_frontTopLeft.y + m_backBottomRight.y) / 2, (m_frontTopLeft.z + m_backBottomRight.z) / 2);
	//Updates the centre and rotations of the collider 
	m_collWorldCentre = Vector3::Transform(m_collLocalCentre, m_world);
	m_collider.Center = m_collWorldCentre;

	XMFLOAT3 euler = MatrixDecomposeYawPitchRoll(m_rot);
	m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z));

	//Storing the Corners of the collider for Toby
	m_globalFrontLeft = Vector3::Transform(Vector3(m_frontTopLeft.x, m_collLocalCentre.y, m_frontTopLeft.z), m_world);
	m_globalFrontRight = Vector3::Transform(Vector3(m_frontTopRight.x, m_collLocalCentre.y, m_frontTopRight.z), m_world);
	m_globalBackLeft = Vector3::Transform(Vector3(m_backBottomLeft.x, m_collLocalCentre.y, m_backBottomLeft.z), m_world);
	m_globalBackRight = Vector3::Transform(Vector3(m_backBottomRight.x, m_collLocalCentre.y, m_backBottomRight.z), m_world);
	m_width = m_frontTopLeft.x - m_frontTopRight.x;
	m_height = m_frontTopLeft.y - m_backBottomLeft.y;
	m_length = m_frontTopLeft.z - m_backBottomLeft.z;
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
	if (m_hasCollider)
	{
		//Updates the centre and rotations of the collider 
		m_collWorldCentre = Vector3::Transform(m_collLocalCentre, m_world);
		m_collider.Center = m_collWorldCentre;

		XMFLOAT3 euler = MatrixDecomposeYawPitchRoll(m_rot);
		m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z));

		//Storing the Corners of the collider for Toby
		m_globalFrontLeft = Vector3::Transform(Vector3(m_frontTopLeft.x, m_collLocalCentre.y, m_frontTopLeft.z), m_world);
		m_globalFrontRight = Vector3::Transform(Vector3(m_frontTopRight.x, m_collLocalCentre.y, m_frontTopRight.z), m_world);
		m_globalBackLeft = Vector3::Transform(Vector3(m_backBottomLeft.x, m_collLocalCentre.y, m_backBottomLeft.z), m_world);
		m_globalBackRight = Vector3::Transform(Vector3(m_backBottomRight.x, m_collLocalCentre.y, m_backBottomRight.z), m_world);

		//Updates the debug collider position and rotation

		m_colliderDebug->SetPos(m_collWorldCentre);
		m_colliderDebug->SetScale(m_collider.Extents);	
		m_colliderDebug->SetYaw(euler.y);
		m_colliderDebug->SetPitch(euler.x);
		m_colliderDebug->SetRoll(euler.z);
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
		if (!m_currentCollisions.empty())
		{
			Vector3 normalised_vel = m_vel;
			normalised_vel.Normalize();
			m_pos -= normalised_vel * 0.05;
			m_vel = Vector3::Zero;
			m_currentCollisions.clear();
		}
	}

	SDKMeshGO3D::Tick(_GSD);

	m_acc = Vector3::Zero;

	updateCollider();
}