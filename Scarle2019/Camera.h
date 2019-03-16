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
		BEHIND = 0,
		FRONT = 1,
		LERP = 2,
		FIRST = 3,
		INDEPENDENT_FIXED = 4,
		INDEPENDENT_LERP = 5,
		ORBIT = 6,
		CINEMATIC = 7,
		DEBUG_CAM = 8,
	};

	Camera(float _width, float _height, float _near, float _far, GameObject3D* _target, Vector3 _dpos);
	~Camera() = default;

	void SetTarget(GameObject3D* _target) { m_targetObject = _target; }
	void SetTarget(Vector3 _target) { m_targetObject = NULL; m_targetPos = _target; }
	void SetDPos(Vector3 _m_dpos) {  m_dpos = _m_dpos; }
	void SetBehav(BEHAVIOUR _behav) { behav = _behav; }
	BEHAVIOUR GetBehav() { return behav; }

	Matrix GetProj() { return m_proj; }
	Matrix GetView() { return m_view; }
	Vector3 GetDeltaPos() { return m_dpos; };

	virtual void Tick();
	virtual void Render() {};



protected:

	GameObject3D* m_targetObject = NULL;
	Vector3 m_targetPos = Vector3::Zero;
	Vector3 m_differnce[8] = { { 0, 0, -10 },{ 5, 0, -5 },{ 10, 0, 0 },{ 10, 0, 0 },{ 5, 0, 5 },{ 0, 0, 10 },{ -5, 0, 5 },{ -10, 0, 0 }};
	int diff_num = 0;
	int look_up_down = -1;
	Vector3 m_differnce2[2]{ { 0, 10, -1 },{ 0, -10, -1 } };
	Matrix m_proj = Matrix::Identity;
	Matrix m_view = Matrix::Identity;
	Matrix rotCam = Matrix::Identity;
	Vector3	m_dpos; 
	std::vector<Vector3> points;
	std::vector<Vector3> rotations;
	BEHAVIOUR behav = BEHAVIOUR::LERP;

	KeybindManager m_keybinds;

	float angle = 0.0f;
	int at = 1;
	float time_out = 5.0f;
	float timer = 0.0f;
	int m_cameraID = 0;
};

