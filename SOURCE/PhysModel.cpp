#include "pch.h"
#include "PhysModel.h"
#include "GameStateData.h"
#include "ServiceLocator.h"
#include <iostream>
#include <fstream>

PhysModel::PhysModel(std::string _filename) :SDKMeshGO3D(_filename)
{
	std::string test = m_filepath.generateConfigFilepath(_filename, m_filepath.MODEL);
	std::ifstream i(test);

	if (i)
	{
		json model_data;
		model_data << i;
		m_hasCollider = model_data["has_box_collider"];

		m_physData.m_scale = model_data["modelscale"];
		SetScale(m_physData.m_scale);
		/*
		For whatever reason, setting this here will cause horrible collision errors...
		SetPos(Vector3(model_data["start_x"], model_data["start_y"], model_data["start_z"]));
		SetRotationInDegrees(Vector3(model_data["rot_x"], model_data["rot_y"], model_data["rot_z"]));
		*/

		if (m_hasCollider)
		{
			m_colliderDebug = new SDKMeshGO3D(_filename + " DEBUG");
			m_colliderDebug->SetScale(m_physData.m_scale);
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
	m_physData.m_localFrontTopLeft = Vector3((float)model_data["collision_box"]["front_top_left"][0] * m_physData.m_scale,
		(float)model_data["collision_box"]["front_top_left"][1] * m_physData.m_scale,
		(float)model_data["collision_box"]["front_top_left"][2] * m_physData.m_scale);

	m_physData.m_localFrontTopRight = Vector3((float)model_data["collision_box"]["front_top_right"][0] * m_physData.m_scale,
		(float)model_data["collision_box"]["front_top_right"][1] * m_physData.m_scale,
		(float)model_data["collision_box"]["front_top_right"][2] * m_physData.m_scale);

	m_physData.m_localFrontBottomLeft = Vector3((float)model_data["collision_box"]["front_bottom_left"][0] * m_physData.m_scale,
		(float)model_data["collision_box"]["front_bottom_left"][1] * m_physData.m_scale,
		(float)model_data["collision_box"]["front_bottom_left"][2] * m_physData.m_scale);

	m_physData.m_localFrontBottomRight = Vector3((float)model_data["collision_box"]["front_bottom_right"][0] * m_physData.m_scale,
		(float)model_data["collision_box"]["front_bottom_right"][1] * m_physData.m_scale,
		(float)model_data["collision_box"]["front_bottom_right"][2] * m_physData.m_scale);

	m_physData.m_localBackTopLeft = Vector3((float)model_data["collision_box"]["back_top_left"][0] * m_physData.m_scale,
		(float)model_data["collision_box"]["back_top_left"][1] * m_physData.m_scale,
		(float)model_data["collision_box"]["back_top_left"][2] * m_physData.m_scale);

	m_physData.m_localBackTopRight = Vector3((float)model_data["collision_box"]["back_top_right"][0] * m_physData.m_scale,
		(float)model_data["collision_box"]["back_top_right"][1] * m_physData.m_scale,
		(float)model_data["collision_box"]["back_top_right"][2] * m_physData.m_scale);

	m_physData.m_localBackBottomLeft = Vector3((float)model_data["collision_box"]["back_bottom_left"][0] * m_physData.m_scale,
		(float)model_data["collision_box"]["back_bottom_left"][1] * m_physData.m_scale,
		(float)model_data["collision_box"]["back_bottom_left"][2] * m_physData.m_scale);

	m_physData.m_localBackBottomRight = Vector3((float)model_data["collision_box"]["back_bottom_right"][0] * m_physData.m_scale,
		(float)model_data["collision_box"]["back_bottom_right"][1] * m_physData.m_scale,
		(float)model_data["collision_box"]["back_bottom_right"][2] * m_physData.m_scale);


	m_physData.m_localCentre = Vector3((m_physData.m_localFrontTopLeft.x + m_physData.m_localFrontTopRight.x) / 2
		, (m_physData.m_localFrontTopLeft.y + m_physData.m_localFrontBottomLeft.y) / 2, (m_physData.m_localFrontTopLeft.z + m_physData.m_localBackTopLeft.z) / 2);

	updateCollider();

	m_physData.m_width = m_physData.m_globalFrontTopLeft.x - m_physData.m_globalFrontTopRight.x;
	m_physData.m_height = m_physData.m_globalFrontTopLeft.y - m_physData.m_globalFrontBottomLeft.y;
	m_physData.m_length = m_physData.m_globalFrontTopLeft.z - m_physData.m_globalBackTopLeft.z;
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
		m_collider.Extents = m_scale * m_physData.m_additionalScale;
		Vector3 euler = MatrixDecomposeYawPitchRoll(m_rot);
		m_collider.Orientation = XMFLOAT4(Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z));
		//Updates the debug collider position and rotation
		m_colliderDebug->SetPos(Vector3::Transform(Vector3(m_physData.m_localCentre.x, m_physData.m_localCentre.y - (m_physData.m_height / 2), m_physData.m_localCentre.z), m_world));
		m_colliderDebug->SetScale(m_physData.m_scale);
		m_colliderDebug->SetYaw(euler.y);
		m_colliderDebug->SetPitch(euler.x);
		m_colliderDebug->SetRoll(euler.z);
		if (debug_print)
		{
			DebugText::print("START OF PHYSMODEL INFO DUMP");
			DebugText::print("Mesh Centre: (" + std::to_string(m_physData.m_worldCentre.x) + ", " + std::to_string(m_physData.m_worldCentre.y) + ", " + std::to_string(m_physData.m_worldCentre.z) + ")");
			DebugText::print("Position: (" + std::to_string(m_pos.x) + ", " + std::to_string(m_pos.y) + ", " + std::to_string(m_pos.z) + ")");
			Vector3 diff = m_physData.m_worldCentre - m_pos;
			DebugText::print("Diff: (" + std::to_string(diff.x) + ", " + std::to_string(diff.y) + ", " + std::to_string(diff.z) + ")");
			DebugText::print("END OF PHYSMODEL INFO DUMP");
		}
	}
}

void PhysModel::Tick()
{
	if (m_physicsOn)
	{
		if (m_acc.Length() > 1)
		{
			bool breakme = false;
		}
		m_vel = m_vel + (Locator::getGSD()->m_dt * (m_acc - (m_drag * m_vel)));
		// Cap the movment speed to its maximum
		if (m_vel.Length() > m_maxSpeed)
		{
			m_vel.Normalize();
			m_vel *= m_maxSpeed;
		}

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

void PhysModel::Reset()
{
	SDKMeshGO3D::Reset();
	Vector3 m_velTotal = Vector3::Zero;
	Vector3 m_vel = Vector3::Zero;
	Vector3 m_gravDirection = Vector3::Down;
	Vector3 m_gravVel = Vector3::Zero;
}

Vector3 PhysModel::applyOffset(Vector3 pos)
{
	return Vector3(pos.x - offset, pos.y, pos.z);
}