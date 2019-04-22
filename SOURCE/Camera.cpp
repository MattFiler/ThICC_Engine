#include "pch.h"
#include "Camera.h"
#include "GameStateData.h"
#include "Player.h"
#include "Keyboard.h"
#include "KeybindManager.h"
#include <math.h>

Camera::Camera(float _width, float _height, float _near, float _far, GameObject3D* _target, Vector3 _dpos)
{
	m_pos = Vector3::Backward;
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, _width / _height, _near, _far);
	m_targetObject = _target;
	m_dpos = _dpos;
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
	Vector up_transform;
	Matrix orientation;
	float rot_lerp;
	float pos_lerp;

	switch (behav)
	{
	case BEHAVIOUR::FOLLOW:
	{
		m_dpos = Vector3{ 0.0f, 3.0f, 10.0f };
		if (m_targetObject)
		{
			orientation = m_targetObject->GetOri();
			rot_lerp = 0.1f;
			pos_lerp = 0.25f;
			look_at_target = m_targetObject->GetPos();
			target_pos = m_targetObject->GetPos() + m_targetObject->GetPos().Transform(m_dpos, orientation);
			up_transform = m_targetObject->GetWorld().Up();
		}
		break;
	}
	case BEHAVIOUR::ORBIT:
	{
		if (m_targetObject)
		{
			m_dpos = Vector3{ 10.0f, 3.0f, 10.0f };
			orientation = m_targetObject->GetOri();
			rot_lerp = 0.05f;
			pos_lerp = 1.f;
			look_at_target = m_targetObject->GetPos();
			up_transform = m_targetObject->GetWorld().Up();
			angle += 1.5f;

			target_pos = m_targetObject->GetPos() + Vector3::Transform({ sin(angle / 57.2958f) * m_dpos.x, m_dpos.y, cos(angle / 57.2958f) * m_dpos.z }, orientation);
		}
		break;
	}
	case BEHAVIOUR::CINEMATIC:
	{
		timer += Locator::getGSD()->m_dt;
		time_out = 4.0f;
		rotCam = Matrix::Identity;

		if (cam_point < points.size())
		{
			target_pos = Vector3::Lerp(points[cam_point][0], points[cam_point][1], timer / time_out);
			m_targetPos = look_points[cam_point];

			if (timer >= time_out)
			{
				timer = 0.0f;
				cam_point++;
			}
		}
		look_at_target = m_targetPos;
		up_transform = Vector3::Up;
		rot_lerp = 0.05f;
		pos_lerp = 1.f;
		break;
	}
	case BEHAVIOUR::FIRST:
	{
		if (m_targetObject)
		{
			m_dpos = Vector3{ 0.0f, 2.0f, -2.5f };
			orientation = m_targetObject->GetOri();
			look_at_target = m_targetObject->GetPos() + m_targetObject->GetPos().Transform(Vector3{ 0.0f, 2.0f, -10.0f }, orientation);
			up_transform = m_targetObject->GetWorld().Up();
			pos_lerp = 1.f;
			rot_lerp = 0.08;
			target_pos = m_targetObject->GetPos() + m_targetObject->GetPos().Transform(m_dpos, orientation);
		}
		else
			m_view = Matrix::CreateLookAt(m_pos, m_targetPos, Vector3::Up);
		break;
	}
	case BEHAVIOUR::INDEPENDENT:
	{
		if (Locator::getID()->m_gamePadState[m_cameraID].IsRightThumbStickLeft())
			angle -= 3.0f;

		if (Locator::getID()->m_gamePadState[m_cameraID].IsRightThumbStickRight())
			angle += 3.0f;

		if (m_targetObject)
		{
			m_dpos = Vector3{ 10.0f, 3.0f, 10.0f };
			orientation = m_targetObject->GetOri();
			rot_lerp = 0.2f;
			pos_lerp = 1.f;
			look_at_target = m_targetObject->GetPos();
			up_transform = m_targetObject->GetWorld().Up();
			target_pos = m_targetObject->GetPos() + Vector3::Transform({ sin(angle / 57.2958f) * m_dpos.x, m_dpos.y, cos(angle / 57.2958f) * m_dpos.z }, orientation);
		}
		break;
	}
	case BEHAVIOUR::RACE_START:
	{
		timer += Locator::getGSD()->m_dt;
		time_out = 3.f;
		Vector3 start_pos = { 5.0f, 1.0f, -5.0f };
		m_dpos = Vector3{ 0.0f, 3.0f, 10.0f };
		if (m_targetObject)
		{
			orientation = m_targetObject->GetOri();
			look_at_target = m_targetObject->GetPos();
			pos_lerp = 1.f;
			rot_lerp = 0.2f;
			up_transform = m_targetObject->GetWorld().Up();
			target_pos = m_targetObject->GetPos() + m_targetObject->GetPos().Transform(Vector3::Lerp(start_pos, m_dpos, timer / time_out), rotCam);
			if (timer >= time_out)
			{
				timer = 0.0f;
				behav = BEHAVIOUR::FOLLOW;
			}
		}
	}
#ifdef _DEBUG
	case BEHAVIOUR::DEBUG_CAM:
	{
		float cam_speed = 40.0f;
		float cam_rot_speed = 0.007f;
		if (m_keybinds.keyHeld("DebugCamSpeedup"))
		{
			cam_speed *= 2;
			cam_rot_speed *= 2;
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
		m_pitch -= cam_rot_speed * mouse_ypos;

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

		if (m_keybinds.keyHeld("DebugCamLookUp"))
			m_pitch -= cam_rot_speed * 10;
		else if (m_keybinds.keyHeld("DebugCamLookDown"))
			m_pitch += cam_rot_speed * 10;

		if (m_keybinds.keyHeld("DebugCamLookLeft"))
			m_yaw -= cam_rot_speed * 10;
		else if (m_keybinds.keyHeld("DebugCamLookRight"))
			m_yaw += cam_rot_speed * 10;

		m_view = Matrix::CreateLookAt(m_pos, m_targetPos, m_pos.Up);

		break;
	}
#endif
	}

#ifdef _DEBUG
	if (behav != BEHAVIOUR::DEBUG_CAM)
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
	time_out = 3.0f;
	timer = 0.0f;
	m_cameraID = 0;
}