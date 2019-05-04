#pragma once
#include "MoveAI.h"
#include <random>

class KartAI :
	public MoveAI
{
public:
	KartAI(PhysModel* _model, ControlledMovement* _move);
	~KartAI();
	bool Update() override;

private:
	bool ShouldSpeedBoost(const ItemType& type);

	float m_lastItemUse = 0;
	float m_timeForNextItem = 0;
	float m_timeElapsed = 0;
	float m_minItemDelay = 5;
	float m_maxItemDelay = 15;

	float m_minDistForBoost = 35;

	std::mt19937 m_generator;
	std::uniform_int_distribution<int> m_probabilityRoll;

	bool m_hasItem = false;
};

