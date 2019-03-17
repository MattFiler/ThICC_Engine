#pragma once
#include "SDKMeshGO3D.h"
#include <json.hpp>
using json = nlohmann::json;

struct PhysModelData 
{
	float scale = 1.0f;
	
	Vector3 m_localCentre;
	Vector3 m_worldCentre;

	float m_width;
	float m_length;
	float m_height;

	//Corner Pos in Local Space
	Vector3 m_localFrontTopLeft;
	Vector3 m_localFrontTopRight;
	Vector3 m_localFrontBottomLeft;
	Vector3 m_localFrontBottomRight;
	Vector3 m_localBackTopLeft;
	Vector3 m_localBackTopRight;
	Vector3 m_localBackBottomLeft;
	Vector3 m_localBackBottomRight;

	//Corner Pos in World Space
	Vector3 m_globalFrontTopLeft;
	Vector3 m_globalFrontTopRight;
	Vector3 m_globalFrontBottomLeft;
	Vector3 m_globalFrontBottomRight;
	Vector3 m_globalBackTopLeft;
	Vector3 m_globalBackTopRight;
	Vector3 m_globalBackBottomLeft;
	Vector3 m_globalBackBottomRight;

};

class PhysModel : public SDKMeshGO3D
{
public:

	 enum m_Corner{FRONT_LEFT, FRONT_RIGHT, BACK_RIGHT, BACK_LEFT};

	PhysModel(string _filename);
	virtual ~PhysModel() = default;

	void initCollider(json &model_data);
	bool hasCollider() {return m_hasCollider;};
	void updateCollider();

	virtual void Tick() override;

	bool		IsPhysicsOn() { return m_physicsOn; }
	float		GetDrag() { return m_drag; }

	void		SetPhysicsOn(bool _physics) { m_physicsOn = _physics; }
	void		TogglePhysics() { m_physicsOn = !m_physicsOn; }
	void		SetDrag(float _drag) { m_drag = _drag; }

	void setMass(float _mass) { m_mass = _mass; };
	float getMass() { return m_mass; };
	Vector3 getVelocity() { return m_vel; };
	Vector3 getVelocityTotal() { return m_velTotal; };
	void setVelocity(Vector3 _vel) { m_vel = _vel; };
	void setVelocityTotal(Vector3 _velTotal) { m_velTotal = _velTotal; };

	BoundingOrientedBox getCollider() { return m_collider; };

	SDKMeshGO3D* getDebugCollider() { return m_colliderDebug; };

	const PhysModelData& data = m_physData;

protected:
	int m_model_id = 0;

	//very basic physics
	float m_mass = 1;
	bool m_physicsOn = false;
	float m_drag = 0.0f;
	Vector3 m_velTotal = Vector3::Zero; // The total velocity to apply each frame
	Vector3 m_vel = Vector3::Zero; // The amount of player added velocity to apply each frame

	Vector3 m_gravDirection = Vector3::Down;
	Vector3 m_gravVel = Vector3::Zero; // The amount of gravity velocity to apply each frame
	float m_maxGrav = 120; // The maximum length of m_gravVel. m_gravVel will be clamped to this
	Vector3 m_acc = Vector3::Zero;
	PhysModelData m_physData;

	bool m_hasCollider = false;
	BoundingOrientedBox m_collider; //Bounding box of the model

	SDKMeshGO3D* m_colliderDebug = nullptr;
	XMFLOAT3 MatrixDecomposeYawPitchRoll(Matrix  mat);

};