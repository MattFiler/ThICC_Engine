#pragma once
#include "GameObject3D.h"
#include "KeybindManager.h"
struct GameStateData;

//This is a basic camera class
//if the pointer to a GameObject3D != NULL this will be targeted at that object
//else it will look at the point m_targetPos

class Camera :
	public GameObject3D
{
public:

	enum class BEHAVIOUR : int
	{
		FOLLOW = 0,
		BACK_FACING = 1,
		FIRST = 3,
		INDEPENDENT = 4,
		ORBIT = 6,
		CINEMATIC = 7,
#ifdef _DEBUG
		DEBUG_CAM = 8,
#endif
		RACE_START = 9,
	};

	Camera(float _width, float _height, float _near, float _far, GameObject3D* _target, Vector3 _dpos);
	~Camera() = default;

	void SetTarget(GameObject3D* _target) { m_targetObject = _target; }
	void SetTarget(Vector3 _target) { m_targetObject = NULL; m_targetPos = _target; }
	void SetDPos(Vector3 _m_dpos) { m_dpos = _m_dpos; }
	void SetBehav(BEHAVIOUR _behav) { behav = _behav; }

	/*
	void SetCinematicPos(std::vector<Vector3> positions);
	void SetCinematicRot(std::vector<Vector3> _rotations);
	*/

	Matrix GetProj() { return m_proj; }
	Matrix GetView() { return m_view; }
	Vector3 GetDeltaPos() { return m_dpos; };
	BEHAVIOUR GetBehav() { return behav; }

	virtual void Tick();
	virtual void Render() {};

	void Reset();

	//Timer debug
	float getTimer() { return timer; };
	void resetTimer() { timer = 0.0f; };



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

	BEHAVIOUR behav;

	KeybindManager m_keybinds;

	float angle = 0.0f;
	int cam_point = 0;
	float time_out = 3.0f;
	float timer = 0.0f;
	int m_cameraID = 0;
	float last_mouse_xpos = 0.0f;
	float last_mouse_ypos = 0.0f;
};