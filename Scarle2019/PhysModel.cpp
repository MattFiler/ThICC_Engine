#include "pch.h"
#include "PhysModel.h"
#include "GameStateData.h"
#include "ServiceLocator.h"
#include <iostream>
#include <fstream>

PhysModel::PhysModel(string _filename) :SDKMeshGO3D(_filename)
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

			m_colliderDebug = new SDKMeshGO3D(_filename + " DEBUG");
			m_colliderDebug->SetScale(m_physData.scale);
		}
	}
}

void PhysModel::initCollider(json &model_data)
{
	/*Gets the the top front left and back bottom right points of the mesh from the json file - finds the average between them to get the centre of the mesh
	and uses (currently) the top front left point to determine the extents (size) of the collider */
	m_hasCollider = true;

	m_physData.m_localFrontTopLeft = Vector3((float)model_data["collision_box"]["front_top_left"][0] * m_physData.scale,
		(float)model_data["collision_box"]["front_top_left"][1] * m_physData.scale,
		(float)model_data["collision_box"]["front_top_left"][2] * m_physData.scale);

	m_physData.m_localFrontTopRight = Vector3((float)model_data["collision_box"]["front_top_right"][0] * m_physData.scale,
		(float)model_data["collision_box"]["front_top_right"][1] * m_physData.scale,
		(float)model_data["collision_box"]["front_top_right"][2] * m_physData.scale);

	m_physData.m_localFrontBottomLeft = Vector3((float)model_data["collision_box"]["front_bottom_left"][0] * m_physData.scale,
		(float)model_data["collision_box"]["front_bottom_left"][1] * m_physData.scale,
		(float)model_data["collision_box"]["front_bottom_left"][2] * m_physData.scale);

	m_physData.m_localFrontBottomRight = Vector3((float)model_data["collision_box"]["front_bottom_right"][0] * m_physData.scale,
		(float)model_data["collision_box"]["front_bottom_right"][1] * m_physData.scale,
		(float)model_data["collision_box"]["front_bottom_right"][2] * m_physData.scale);

	m_physData.m_localBackBottomLeft = Vector3((float)model_data["collision_box"]["back_bottom_left"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_left"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_left"][2] * m_physData.scale);

	m_physData.m_localBackBottomRight = Vector3((float)model_data["collision_box"]["back_bottom_right"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_right"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_right"][2] * m_physData.scale);

	m_physData.m_localBackTopLeft = Vector3((float)model_data["collision_box"]["back_top_left"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_top_left"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_top_left"][2] * m_physData.scale);

	m_physData.m_localBackTopRight = Vector3((float)model_data["collision_box"]["back_top_right"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_top_right"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_top_right"][2] * m_physData.scale);


	m_physData.m_localCentre = Vector3((m_physData.m_localFrontTopLeft.x + m_physData.m_localFrontTopRight.x) / 2
		, (m_physData.m_localFrontTopLeft.y + m_physData.m_localFrontBottomLeft.y) / 2, (m_physData.m_localFrontTopLeft.z + m_physData.m_localBackTopLeft.z) / 2);
	//Updates the centre and rotations of the collider 
	m_physData.m_worldCentre = Vector3::Transform(m_physData.m_localCentre, m_world);
	m_collider.Center = m_physData.m_worldCentre;

	XMFLOAT3 euler = MatrixDecomposeYawPitchRoll(m_rot);
	m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y , euler.x, euler.z));

	//Storing the Corners of the collider for Toby
	m_physData.m_globalFrontCentreLeft = Vector3::Transform(Vector3(m_physData.m_localFrontTopLeft.x, m_physData.m_localCentre.y, m_physData.m_localFrontTopLeft.z), m_world);
	m_physData.m_globalFrontCentreRight = Vector3::Transform(Vector3(m_physData.m_localFrontTopRight.x, m_physData.m_localCentre.y, m_physData.m_localFrontTopRight.z), m_world);
	m_physData.m_globalBackCentreLeft = Vector3::Transform(Vector3(m_physData.m_localBackBottomLeft.x, m_physData.m_localCentre.y, m_physData.m_localBackBottomLeft.z), m_world);
	m_physData.m_globalBackCentreRight = Vector3::Transform(Vector3(m_physData.m_localBackBottomRight.x, m_physData.m_localCentre.y, m_physData.m_localBackBottomRight.z), m_world);

	m_physData.m_globalFrontTopLeft = Vector3::Transform(m_physData.m_localFrontTopLeft, m_world);
	m_physData.m_globalFrontTopRight = Vector3::Transform(m_physData.m_localFrontTopRight, m_world);
	m_physData.m_globalFrontBottomLeft = Vector3::Transform(m_physData.m_localFrontBottomLeft, m_world);
	m_physData.m_globalFrontBottomRight = Vector3::Transform(m_physData.m_localFrontBottomRight, m_world);
	m_physData.m_globalBackTopLeft = Vector3::Transform(m_physData.m_localBackTopLeft, m_world);
	m_physData.m_globalBackTopRight = Vector3::Transform(m_physData.m_localBackTopRight, m_world);
	m_physData.m_globalBackBottomLeft = Vector3::Transform(m_physData.m_localBackBottomLeft, m_world);
	m_physData.m_globalBackBottomRight = Vector3::Transform(m_physData.m_localBackBottomRight, m_world);

	m_physData.m_width = m_physData.m_localFrontTopLeft.x - m_physData.m_localFrontTopRight.x;
	m_physData.m_height = m_physData.m_localFrontTopLeft.y - m_physData.m_localBackBottomLeft.y;
	m_physData.m_length = m_physData.m_localFrontTopLeft.z - m_physData.m_localBackBottomLeft.z;
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
		m_physData.m_worldCentre = Vector3::Transform(m_physData.m_localCentre, m_world);
		m_collider.Center = m_physData.m_worldCentre;

		XMFLOAT3 euler = MatrixDecomposeYawPitchRoll(m_rot);
		m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y , euler.x, euler.z));

		//Storing the Corners of the collider for Toby
		m_physData.m_globalFrontCentreLeft = Vector3::Transform(Vector3(m_physData.m_localFrontTopLeft.x, m_physData.m_localCentre.y, m_physData.m_localFrontTopLeft.z), m_world);
		m_physData.m_globalFrontCentreRight = Vector3::Transform(Vector3(m_physData.m_localFrontTopRight.x, m_physData.m_localCentre.y, m_physData.m_localFrontTopRight.z), m_world);
		m_physData.m_globalBackCentreLeft = Vector3::Transform(Vector3(m_physData.m_localBackBottomLeft.x, m_physData.m_localCentre.y, m_physData.m_localBackBottomLeft.z), m_world);
		m_physData.m_globalBackCentreRight = Vector3::Transform(Vector3(m_physData.m_localBackBottomRight.x, m_physData.m_localCentre.y, m_physData.m_localBackBottomRight.z), m_world);

		m_physData.m_globalFrontTopLeft = Vector3::Transform(m_physData.m_localFrontTopLeft, m_world);
		m_physData.m_globalFrontTopRight = Vector3::Transform(m_physData.m_localFrontTopRight, m_world);
		m_physData.m_globalFrontBottomLeft = Vector3::Transform(m_physData.m_localFrontBottomLeft, m_world);
		m_physData.m_globalFrontBottomRight = Vector3::Transform(m_physData.m_localFrontBottomRight, m_world);
		m_physData.m_globalBackTopLeft = Vector3::Transform(m_physData.m_localBackTopLeft, m_world);
		m_physData.m_globalBackTopRight = Vector3::Transform(m_physData.m_localBackTopRight, m_world);
		m_physData.m_globalBackBottomLeft = Vector3::Transform(m_physData.m_localBackBottomLeft, m_world);
		m_physData.m_globalBackBottomRight = Vector3::Transform(m_physData.m_localBackBottomRight, m_world);

		//Updates the debug collider position and rotation
		m_colliderDebug->SetPos(Vector3(m_physData.m_worldCentre.x, m_physData.m_worldCentre.y - (m_physData.m_height / 2), m_physData.m_worldCentre.z));
		//m_colliderDebug->SetScale(m_collider.Extents);	
		m_colliderDebug->SetYaw(euler.y);
		m_colliderDebug->SetPitch(euler.x);
		m_colliderDebug->SetRoll(euler.z);
	}
}

void PhysModel::Tick()
{
	if (m_physicsOn)
	{
		m_vel = m_vel + Locator::getGSD()->m_dt * (m_acc - m_drag*m_vel);

		m_gravVel = m_gravVel + Locator::getGSD()->m_dt * (m_gravDirection);
		if (m_gravVel.Length() > m_maxGrav)
		{
			m_gravVel.Normalize();
			m_gravVel *= m_maxGrav;
		}
		
		m_velTotal = m_vel + m_gravVel;

		m_pos += Locator::getGSD()->m_dt * m_velTotal;
	}

	SDKMeshGO3D::Tick();

	m_acc = Vector3::Zero;

	updateCollider();
}


