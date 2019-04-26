#pragma once
#include "pch.h"

struct ItemGrowthData
{
	Vector3 m_startScale;
	Vector3 m_endScale;
	float m_scaleMulti = 0;
	float m_growthSpeed = 0;
	float m_shrinkSpeed = 0;
	float m_scalePercent = 0;
	float m_growthTimeElapsed = 0;
	float m_growthDuration = 0;
	enum ScaleState { GROW, MAINTIAIN, SHRINK } m_scaleState = GROW;
};