#pragma once
#include "PhysModel.h"


struct Collision
{
	PhysModel* m_model1 = nullptr;
	PhysModel* m_model2 = nullptr;

	Vector3 m_collisionNormal = Vector3::Zero;
	Vector3 m_penertation_depth = Vector3::Zero;

	enum m_FrontBack {FRONT, BACK, NONE_FB};
	enum m_LeftRight {LEFT, RIGHT, NONE_LR};

	m_FrontBack m_frontBack = m_FrontBack::NONE_FB;
	m_LeftRight m_leftRight = m_LeftRight::NONE_LR;
};