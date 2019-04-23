#include "pch.h"
#include "Camera.h"
#include "GameStateData.h"
#include "Player.h"
#include "Keyboard.h"
#include "KeybindManager.h"
#include <math.h>

Camera::Camera(float _width, float _height, float _near, float _far, Vector3 _dpos, GameObject3D * _target, Behavior _behav = Behavior::FOLLOW) : behavior (_behav)
{

	m_pos = Vector3::Backward;
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, _width / _height, _near, _far);

	m_targetObject = _target;



	std::map <Behavior, std::string> behavior_map;
	behavior_map[Behavior::FOLLOW] = "FOLLOW";
	behavior_map[Behavior::BACK_FACING] = "BACK_FACING";
	behavior_map[Behavior::FIRST] = "FIRST";
	behavior_map[Behavior::ORBIT] = "ORBIT";
	behavior_map[Behavior::INDEPENDENT] = "INDEPENDENT";
	behavior_map[Behavior::CINEMATIC] = "CINEMATIC";
#if _DEBUG
	behavior_map[Behavior::DEBUG_CAM] = "DEBUG_CAM";
#endif 

	//Read in track config
	std::ifstream i("DATA/CONFIGS/CAMERA_CONFIG.JSON");
	json m_camera_configs;
	m_camera_configs << i;

	for (size_t i = 0; i < behavior_map.size(); i++)
	{
		setUpCameras(m_camera_configs, behavior_map[static_cast<Camera::Behavior>(i)]);
	}
	//setUpCameras(m_camera_configs, _target, "FOLLOW");

	//m_dpos = _dpos;
}

void Camera::setUpCameras(json &m_camera_configs, std::string camera_type)
{
	if (camera_type != "DEBUG_CAM")
	{
		rotation_lerps.push_back(m_camera_configs[camera_type]["rotation_lerp"]);
		position_lerps.push_back(m_camera_configs[camera_type]["position_lerp"]);
		target_positions.push_back(Vector3{ m_camera_configs[camera_type]["target_position"][0],
			m_camera_configs[camera_type]["target_position"][1],
			m_camera_configs[camera_type]["target_position"][2] });
		look_at_positions.push_back(Vector3{ m_camera_configs[camera_type]["look_at_position"][0],
			m_camera_configs[camera_type]["look_at_position"][1],
			m_camera_configs[camera_type]["look_at_position"][2] });
		camera_offsets.push_back(Vector3{ m_camera_configs[camera_type]["camera_offset"][0],
			m_camera_configs[camera_type]["camera_offset"][1],
			m_camera_configs[camera_type]["camera_offset"][2] });
		look_at_offsets.push_back(Vector3{ m_camera_configs[camera_type]["look_at_offset"][0],
			m_camera_configs[camera_type]["look_at_offset"][1],
			m_camera_configs[camera_type]["look_at_offset"][2] });

	if (camera_type == "ORBIT")
		orbit_spin_amount = m_camera_configs[camera_type]["spin_amount"];

	else if (camera_type == "INDEPENDENT")
		indep_spin_amount = m_camera_configs[camera_type]["spin_amount"];

	else if (camera_type == "CINEMATIC")
		cine_time_out = m_camera_configs[camera_type]["timeout"];
	}
	else
	{
		cam_speed = m_camera_configs[camera_type]["camera_speed"];
		cam_rot_speed = m_camera_configs[camera_type]["camera_rot_speed"];
	}
}

/*
void Camera::SetCinematicPos(std::vector<Vector3> positions)
{
	int x = 0;
	for (int i = 0; i < positions.size(); i+=2)
	{
		std::array<Vector3, 2> array_points;
		array_points[x] = positions[i];
		array_points[x+1] = positions[i+1];
		points.push_back(array_points);
	}
}

void Camera::SetCinematicRot(std::vector<Vector3> _rotations)
{
	int x = 0;
	for (int i = 0; i < _rotations.size(); i += 2)
	{
		std::array<Vector3, 2> array_points;
		array_points[x] = _rotations[i];
		array_points[x + 1] = _rotations[i + 1];
		rotations.push_back(array_points);
	}
}
*/

void Camera::Tick()
{
	Vector3 look_at_target;
	Vector3 target_pos;
	Vector3 up_transform;
	Matrix orientation;
	float rot_lerp;
	float pos_lerp;

	if (behavior != Behavior::DEBUG_CAM)
	{
		m_dpos = camera_offsets[static_cast<int>(behavior)];
		orientation = m_targetObject ? m_targetObject->GetOri() : GetOri();
		//look_at_target = m_targetObject ? m_targetObject->GetPos() : look_at_positions[static_cast<int>(behavior)];
		look_at_target = (m_targetObject ? m_targetObject->GetPos() : look_at_positions[static_cast<int>(behavior)]) + Vector3::Transform(look_at_offsets[static_cast<int>(behavior)], orientation);
		target_pos = (m_targetObject ? m_targetObject->GetPos() : target_positions[static_cast<int>(behavior)]) + Vector3::Transform(m_dpos, orientation);
		up_transform = m_targetObject ? m_targetObject->GetWorld().Up() : Vector3::Up;
		rot_lerp = rotation_lerps[static_cast<int>(behavior)];
		pos_lerp = position_lerps[static_cast<int>(behavior)];
	}

	switch (behavior)
	{
	case Behavior::ORBIT:
	{
		angle += (orbit_spin_amount * Locator::getGSD()->m_dt);
		target_pos = (m_targetObject ? m_targetObject->GetPos() : target_positions[static_cast<int>(behavior)]) + 
			Vector3::Transform({ sin(angle / 57.2958f) * m_dpos.x, m_dpos.y, cos(angle / 57.2958f) * m_dpos.z }, orientation);

		break;
	}
	case Behavior::CINEMATIC:
	{
		timer += Locator::getGSD()->m_dt;
		look_at_target = m_targetPos;
		up_transform = Vector3::Up;

		if (cam_point < points.size())
		{
			target_pos = Vector3::Lerp(points[cam_point][0], points[cam_point][1], timer / cine_time_out);
			m_targetPos = look_points[cam_point];

			if (timer >= cine_time_out)
			{
				timer = 0.0f;
				cam_point++;
			}
		}
		break;
	}
	case Behavior::INDEPENDENT:
	{
		if (Locator::getID()->m_gamePadState[m_cameraID].IsRightThumbStickLeft())
			indep_angle_x += indep_spin_amount;
		else if (Locator::getID()->m_gamePadState[m_cameraID].IsRightThumbStickRight())
			indep_angle_x -= indep_spin_amount;

		if (Locator::getID()->m_gamePadState[m_cameraID].IsRightThumbStickUp() && indep_angle_y > -88)
			indep_angle_y -= indep_spin_amount;
		else if (Locator::getID()->m_gamePadState[m_cameraID].IsRightThumbStickDown() && indep_angle_y < 88)
			indep_angle_y += indep_spin_amount;

		target_pos = m_targetObject->GetPos() + Vector3::Transform({ sin(indep_angle_x / 57.2958f) * m_dpos.x, sin(indep_angle_y / 57.2958f) * m_dpos.y, cos(indep_angle_x / 57.2958f) * m_dpos.z }, orientation);
		break;
	}
#ifdef _DEBUG
	case Behavior::DEBUG_CAM:
	{
		if (m_keybinds.keyHeld("DebugCamSpeedup"))
		{
			cam_speed *= 1.1f;
		}

		Vector3 forwardMove = cam_speed * m_world.Forward();
		Vector3 rightMove = cam_speed * m_world.Right();
		Vector3 upMove = cam_speed * m_world.Up();
		Matrix rotMove = Matrix::CreateRotationY(m_yaw);

		forwardMove = Vector3::Transform(forwardMove, rotMove);
		rightMove = Vector3::Transform(rightMove, rotMove);
		upMove = Vector3::Transform(upMove, rotMove);
		m_targetPos = m_pos + forwardMove;


		float mouse_xpos = last_mouse_xpos - Locator::getID()->m_mouseState.x;
		float mouse_ypos = last_mouse_ypos - Locator::getID()->m_mouseState.y;
		//m_yaw -= cam_rot_speed * Locator::getID()->m_mouseState.x;
		//m_pitch -= cam_rot_speed * Locator::getID()->m_mouseState.y;
		m_yaw += cam_rot_speed * mouse_xpos;
		m_pitch += cam_rot_speed * mouse_ypos;

		if (m_pitch > 1.5f)
			m_pitch = 1.5f;
		else if (m_pitch < -1.5f)
			m_pitch = -1.5f;

		last_mouse_xpos = Locator::getID()->m_mouseState.x;
		last_mouse_ypos = Locator::getID()->m_mouseState.y;

		if (m_keybinds.keyHeld("DebugCamFor"))
		{
			m_pos += Locator::getGSD()->m_dt * forwardMove;
			m_targetPos += Locator::getGSD()->m_dt * forwardMove;
		}
		else if (m_keybinds.keyHeld("DebugCamBack"))
		{
			m_pos -= Locator::getGSD()->m_dt * forwardMove;
			m_targetPos -= Locator::getGSD()->m_dt * forwardMove;
		}

		if (m_keybinds.keyHeld("DebugCamLeft"))
		{
			m_pos -= Locator::getGSD()->m_dt * rightMove;
			m_targetPos -= Locator::getGSD()->m_dt * rightMove;
		}
		else if (m_keybinds.keyHeld("DebugCamRight"))
		{
			m_pos += Locator::getGSD()->m_dt * rightMove;
			m_targetPos += Locator::getGSD()->m_dt * rightMove;
		}

		if (m_keybinds.keyHeld("DebugCamUp"))
		{
			m_pos -= Locator::getGSD()->m_dt * upMove;
			m_targetPos -= Locator::getGSD()->m_dt * upMove;
		}
		else if (m_keybinds.keyHeld("DebugCamDown"))
		{
			m_pos += Locator::getGSD()->m_dt * upMove;
			m_targetPos += Locator::getGSD()->m_dt * upMove;
		}

		if (m_keybinds.keyHeld("DebugCamLookUp") && m_pitch > -1.5)
			m_pitch += cam_rot_speed;
		else if (m_keybinds.keyHeld("DebugCamLookDown") && m_pitch < 1.5)
			m_pitch -= cam_rot_speed;

		if (m_keybinds.keyHeld("DebugCamLookLeft"))
			m_yaw += cam_rot_speed;
		else if (m_keybinds.keyHeld("DebugCamLookRight"))
			m_yaw -= cam_rot_speed;

		m_view = Matrix::CreateLookAt(m_pos, m_targetPos, m_pos.Up);

		break;
	}
#endif
	}

#ifdef _DEBUG
	if (behavior != Behavior::DEBUG_CAM)
	{
		m_view = Matrix::CreateLookAt(m_pos, look_at_target, up_transform);
		if (rotCam != orientation)
			rotCam = Matrix::Lerp(rotCam, orientation, rot_lerp);

		if (m_pos != target_pos)
			m_pos = Vector3::Lerp(m_pos, target_pos, pos_lerp);
	}

	//Debug output player location - useful for setting up spawns
	if (m_keybinds.keyPressed("Debug Print Camera Location")) {
		DebugText::print("CAMERA POSITION: (" + std::to_string(m_pos.x) + ", " + std::to_string(m_pos.y) + ", " + std::to_string(m_pos.z) + ")");
	}
#else
	m_view = Matrix::CreateLookAt(m_pos, look_at_target, up_transform);
	if (rotCam != orientation)
		rotCam = Matrix::Lerp(rotCam, orientation, rot_lerp);

	if (m_pos != target_pos)
		m_pos = Vector3::Lerp(m_pos, target_pos, pos_lerp);
#endif

	//DebugText::print(std::to_string(timer));

	GameObject3D::Tick();
}

/* reset */
void Camera::Reset() {
	angle = 0.0f;
	cam_point = 0;
	cine_time_out = 3.0f;
	timer = 0.0f;
	m_cameraID = 0;
}