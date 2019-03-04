#pragma once
#include "SDKMeshGO3D.h"
#include <json.hpp>
using json = nlohmann::json;

class PhysModel : public SDKMeshGO3D
{
public:

	PhysModel(RenderData* _RD, string _filename);
	void initCollider(json &model_data);
	virtual ~PhysModel();

	virtual void Tick(GameStateData* _GSD) override;

	void updateCollider();


	bool		IsPhysicsOn() { return m_physicsOn; }
	float		GetDrag() { return m_drag; }

	void		SetPhysicsOn(bool _physics) { m_physicsOn = _physics; }
	void		TogglePhysics() { m_physicsOn = !m_physicsOn; }
	void		SetDrag(float _drag) { m_drag = _drag; }

	BoundingOrientedBox getCollider() { return m_collider; };
	void setCollided(bool _collided) { m_collided = _collided; };

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

	bool m_has_collider = false;
	XMFLOAT3 m_local_centre;//Local Centre of the mesh
	XMFLOAT3 m_world_centre;//World Centre of the mesh and the centre of the bounding box
	BoundingOrientedBox m_collider; //Bounding box of the model
	bool m_collided = false; //True if bounding box is inside another

};
