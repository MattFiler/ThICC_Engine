#pragma once
#include "pch.h"
#include "Constants.h"

#include <vector>
#include <map>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

struct ThICC_CameraData
{

	void init()
	{
		//Read in track config
		std::ifstream i("DATA/CONFIGS/CAMERA_CONFIG.JSON");
		json m_camera_configs;
		m_camera_configs << i;

		std::map <CameraType, std::string> type_map;
		type_map[CameraType::FOLLOW] = "FOLLOW";
		type_map[CameraType::BACK_FACING] = "BACK_FACING";
		type_map[CameraType::FIRST] = "FIRST";
		type_map[CameraType::ORBIT] = "ORBIT";
		type_map[CameraType::INDEPENDENT] = "INDEPENDENT";
		type_map[CameraType::CINEMATIC] = "CINEMATIC";
#if _DEBUG			
		type_map[CameraType::DEBUG_CAM] = "DEBUG_CAM";
#endif 

		fov = m_camera_configs["GENERAL"]["f.o.v"];
		m_near = m_camera_configs["GENERAL"]["view_near"];
		m_far = m_camera_configs["GENERAL"]["view_far"];

		for (size_t i = 0; i < type_map.size(); i++)
		{
			CameraType casted_i = static_cast<CameraType>(i);
			if (type_map[casted_i] != "DEBUG_CAM")
			{
				rotation_lerps.push_back(m_camera_configs[type_map[casted_i]]["rotation_lerp"]);
				position_lerps.push_back(m_camera_configs[type_map[casted_i]]["position_lerp"]);
				target_positions.push_back(DirectX::SimpleMath::Vector3{ m_camera_configs[type_map[casted_i]]["target_position"][0],
					m_camera_configs[type_map[casted_i] ]["target_position"][1],
					m_camera_configs[type_map[casted_i] ]["target_position"][2] });
				look_at_positions.push_back(DirectX::SimpleMath::Vector3{ m_camera_configs[type_map[casted_i]]["look_at_position"][0],
					m_camera_configs[type_map[casted_i] ]["look_at_position"][1],
					m_camera_configs[type_map[casted_i] ]["look_at_position"][2] });
				camera_offsets.push_back(DirectX::SimpleMath::Vector3{ m_camera_configs[type_map[casted_i]]["camera_offset"][0],
					m_camera_configs[type_map[casted_i] ]["camera_offset"][1],
					m_camera_configs[type_map[casted_i] ]["camera_offset"][2] });
				look_at_offsets.push_back(DirectX::SimpleMath::Vector3{ m_camera_configs[type_map[casted_i]]["look_at_offset"][0],
					m_camera_configs[type_map[casted_i] ]["look_at_offset"][1],
					m_camera_configs[type_map[casted_i] ]["look_at_offset"][2] });

				if (type_map[casted_i] == "ORBIT")
					orbit_spin_amount = m_camera_configs[type_map[casted_i]]["spin_amount"];

				else if (type_map[casted_i] == "INDEPENDENT")
					indep_spin_amount = m_camera_configs[type_map[casted_i]]["spin_amount"];

				else if (type_map[casted_i] == "CINEMATIC")
					cine_time_out = m_camera_configs[type_map[casted_i]]["timeout"];
			}
			else
			{
				cam_speed = m_camera_configs[type_map[casted_i]]["camera_speed"];
				cam_rot_speed = m_camera_configs[type_map[casted_i]]["camera_rot_speed"];
			}
		}
	}

	std::vector<DirectX::SimpleMath::Vector3> camera_offsets;
	std::vector<DirectX::SimpleMath::Vector3> look_at_positions;
	std::vector<DirectX::SimpleMath::Vector3> target_positions;
	std::vector<DirectX::SimpleMath::Vector3> look_at_offsets;
	std::vector<float> f_o_vs;
	std::vector<float> rotation_lerps;
	std::vector<float> position_lerps;

	float fov;
	float m_near;
	float m_far;

	// Independent Cam variables
	float indep_spin_amount = 0.0f;

	// Orbit Cam varibles
	float orbit_spin_amount = 0.0f;

	// Cinematic Cam variables
	float cine_time_out = 0.0f;

	// Debug Cam variables
	float cam_speed;
	float cam_rot_speed;
};