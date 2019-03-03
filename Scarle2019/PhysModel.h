#pragma once
#include "SDKMeshGO3D.h"

class PhysModel : public SDKMeshGO3D
{
public:

	PhysModel(RenderData* _RD, string _filename);
	virtual ~PhysModel();

	virtual void Tick(GameStateData* _GSD) override;


	bool		IsPhysicsOn() { return m_physicsOn; }
	float		GetDrag() { return m_drag; }

	void		SetPhysicsOn(bool _physics) { m_physicsOn = _physics; }
	void		TogglePhysics() { m_physicsOn = !m_physicsOn; }
	void		SetDrag(float _drag) { m_drag = _drag; }

protected:

	//very basic physics
	bool m_physicsOn = false;
	float m_drag = 0.0f;
	Vector3 m_velTotal = Vector3::Zero; // The total velocity to apply each frame
	Vector3 m_vel = Vector3::Zero; // The amount of player added velocity to apply each frame

	Vector3 m_gravDirection = Vector3::Down;
	Vector3 m_gravVel = Vector3::Zero; // The amount of gravity velocity to apply each frame
	float m_maxGrav = 20; // The maximum length of m_gravVel. m_gravVel will be clamped to this
	Vector3 m_acc = Vector3::Zero;

};
