#pragma once
#include "GameObject3D.h"
struct GameStateData;

//This is a basic camera class
//if the pointer to a GameObject3D != NULL this will be targeted at that object
//else it will look at the point m_targetPos

class Camera :
	public GameObject3D
{
public:
	Camera(float _width, float _height, float _near, float _far, GameObject3D* _target, Vector3 _dpos);
	virtual ~Camera();

	void SetTarget(GameObject3D* _target) { m_targetObject = _target; }
	void SetTarget(Vector3 _target) { m_targetObject = NULL; m_targetPos = _target; }

	Matrix GetProj() { return m_proj; }
	Matrix GetView() { return m_view; }

	virtual void Tick(GameStateData* _GSD);
	virtual void Render(RenderData* _RD) {};

	Vector3 getDeltaPos() { return m_dpos; };

	enum class CAMERA_TYPE : int
	{
		BEHIND = 0,
		SPIN = 1,
		CINEMATIC = 2,
		
	};


protected:
	GameObject3D* m_targetObject = NULL;
	Vector3 m_targetPos = Vector3::Zero;
	Matrix m_proj = Matrix::Identity;
	Matrix m_view = Matrix::Identity;
	Vector3	m_dpos; //I'll lurk this far behind and away from it
};

