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
			m_colliderDebug = new SDKMeshGO3D(_filename + " DEBUG");
			m_colliderDebug->SetScale(m_physData.scale);
			initCollider(model_data);

		}
	}
}

void PhysModel::initCollider(json &model_data)
{
	/*Gets the the top front left and back bottom right points of the mesh from the json file - finds the average between them to get the centre of the mesh
	and uses (currently) the top front left point to determine the extents (size) of the collider */
	m_hasCollider = true;
	float offest = 4;
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

	m_physData.m_localBackTopLeft = Vector3((float)model_data["collision_box"]["back_top_left"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_top_left"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_top_left"][2] * m_physData.scale);

	m_physData.m_localBackTopRight = Vector3((float)model_data["collision_box"]["back_top_right"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_top_right"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_top_right"][2] * m_physData.scale);

	m_physData.m_localBackBottomLeft = Vector3((float)model_data["collision_box"]["back_bottom_left"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_left"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_left"][2] * m_physData.scale);

	m_physData.m_localBackBottomRight = Vector3((float)model_data["collision_box"]["back_bottom_right"][0] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_right"][1] * m_physData.scale,
		(float)model_data["collision_box"]["back_bottom_right"][2] * m_physData.scale);


	m_physData.m_localCentre = Vector3((m_physData.m_localFrontTopLeft.x + m_physData.m_localFrontTopRight.x) / 2
		, (m_physData.m_localFrontTopLeft.y + m_physData.m_localFrontBottomLeft.y) / 2, (m_physData.m_localFrontTopLeft.z + m_physData.m_localBackTopLeft.z) / 2);

	updateCollider();
	m_collider.Extents = Vector3(m_physData.scale, m_physData.scale,m_physData.scale);

	m_physData.m_width = m_physData.m_globalFrontTopLeft.x - m_physData.m_globalFrontTopRight.x;
	m_physData.m_height = m_physData.m_globalFrontTopLeft.y - m_physData.m_globalFrontBottomLeft.y;
	m_physData.m_length = m_physData.m_globalFrontTopLeft.z - m_physData.m_globalBackTopLeft.z;
}

Vector3 PhysModel::MatrixDecomposeYawPitchRoll(Matrix  mat)
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

void PhysModel::updateCollider()
{
	if (m_hasCollider)
	{
		m_physData.m_globalFrontTopLeft = applyOffset(Vector3::Transform(m_physData.m_localFrontTopLeft, m_world));
		m_physData.m_globalFrontTopRight = applyOffset(Vector3::Transform(m_physData.m_localFrontTopRight, m_world));
		m_physData.m_globalFrontBottomLeft = applyOffset(Vector3::Transform(m_physData.m_localFrontBottomLeft, m_world));
		m_physData.m_globalFrontBottomRight = applyOffset(Vector3::Transform(m_physData.m_localFrontBottomRight, m_world));
		m_physData.m_globalBackTopLeft = applyOffset(Vector3::Transform(m_physData.m_localBackTopLeft, m_world));
		m_physData.m_globalBackTopRight = applyOffset(Vector3::Transform(m_physData.m_localBackTopRight, m_world));
		m_physData.m_globalBackBottomLeft = applyOffset(Vector3::Transform(m_physData.m_localBackBottomLeft, m_world));
		m_physData.m_globalBackBottomRight = applyOffset(Vector3::Transform(m_physData.m_localBackBottomRight, m_world));

		m_physData.m_worldCentre = Vector3::Transform(m_physData.m_localCentre, m_world);
		m_collider.Center = m_physData.m_worldCentre;
		Vector3 euler = MatrixDecomposeYawPitchRoll(m_rot);
		m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y , euler.x, euler.z));
		//Updates the debug collider position and rotation
		m_colliderDebug->SetPos(Vector3::Transform(Vector3(m_physData.m_localCentre.x, m_physData.m_localCentre.y - (m_physData.m_height / 2), m_physData.m_localCentre.z), m_world));
		m_colliderDebug->SetScale(m_collider.Extents);	

		m_colliderDebug->SetYaw(euler.y);
		m_colliderDebug->SetPitch(euler.x);
		m_colliderDebug->SetRoll(euler.z);

		if (debug_print)
		{
			std::cout << "Mesh Centre: X: " << std::to_string(m_physData.m_worldCentre.x) << " Y: " << std::to_string(m_physData.m_worldCentre.y) << " Z: " << std::to_string(m_physData.m_worldCentre.z) << std::endl;
			std::cout << "Position: X: " << std::to_string(m_pos.x) << " Y: " << std::to_string(m_pos.y) << " Z: " << std::to_string(m_pos.z) << std::endl;
			Vector3 diff = m_physData.m_worldCentre - m_pos;
			std::cout << "Diff: X: " << std::to_string(diff.x) << " Y: " << std::to_string(diff.y) << " Z: " << std::to_string(diff.z) << std::endl;
		}
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


Vector3 PhysModel::applyOffset(Vector3 pos)
{
	return Vector3(pos.x - offset, pos.y, pos.z);
}