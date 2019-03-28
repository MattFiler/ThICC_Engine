#pragma once
#include "PhysModel.h"



struct Collision
{
	PhysModel* m_model1 = nullptr;
	PhysModel* m_model2 = nullptr;

	Vector3 m_collisionNormal = Vector3::Zero;
};