#pragma once
#include "GameObject3D.h"
#include "KeybindManager.h"
#include "Constants.h"
struct GameStateData;

//This is a basic camera class
//if the pointer to a GameObject3D != NULL this will be targeted at that object
//else it will look at the point m_targetPos

class Camera :
	public GameObject3D
{
public:

	Camera(float _width, float _height, Vector3 _dpos, GameObject3D * _target, CameraType _behav);
	~Camera() = default;

	void SetTarget(GameObject3D* _target) { m_targetObject = _target; }
	void SetTarget(Vector3 _target) { m_targetObject = NULL; m_targetPos = _target; }
	void SetDPos(Vector3 _m_dpos) { m_dpos = _m_dpos; }
	void SetType(CameraType _behav) { cam_type = _behav; }

	/*
	void SetCinematicPos(std::vector<Vector3> positions);
	void SetCinematicRot(std::vector<Vector3> _rotations);
	*/

	Matrix GetProj() { return m_proj; }
	Matrix GetView() { return m_view; }
	Vector3 GetDeltaPos() { return m_dpos; };
	CameraType GetType() { return cam_type; }

	virtual void Tick();
	virtual void Render() {};

	void Reset();

	//Timer debug
	float getTimer() { return timer; };
	void resetTimer() { timer = 0.0f; };
	void setAngle(float _angle) { angle = _angle; }


protected:

	GameObject3D* m_targetObject = NULL;
	Vector3 m_targetPos = Vector3::Zero;
	Matrix m_proj = Matrix::Identity;
	Matrix m_view = Matrix::Identity;
	Matrix rotCam = Matrix::Identity;
	Vector3	m_dpos;

	std::vector<std::array<Vector3, 2>> points{
		{
			Vector3{ 61, 291, 167 },
			Vector3{ -56, 291, 74 }
		},
		{
			Vector3{ 426, 333, -455 },
			Vector3{ 408, 313, -426 }
		},
		{
			Vector3{ 4, 158, -46 },
			Vector3{ 8, 166, -63 }
		}
	};
	std::vector<Vector3> look_points{
		Vector3{ -336, 344, 119 },
		Vector3{ 27, 156, -89 },
		Vector3{ 30, 152, 30 }
	};
	std::vector<Vector3> rotations;

	/*
	std::vector<std::array<Vector3, 2>> points;
	std::vector<std::array<Vector3, 2>> rotations;
	std::vector<Vector3> look_points;
	*/

	CameraType cam_type;

	KeybindManager m_keybinds;

	int m_cameraID = 0;

	// Independent Cam variables
	float indep_angle_x = 0.0f;
	float indep_angle_y = 60.0f;

	// Orbit Cam varibles
	float angle = 0.0f;

	// Cinematic Cam variables
	int cam_point = 0;
	float timer = 0.0f;

	// Debug Cam variables
	float last_mouse_xpos = 0.0f;
	float last_mouse_ypos = 0.0f;
};