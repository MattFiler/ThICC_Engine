#pragma once
#include "pch.h"

struct ItemGrowthData
{
	Vector3 m_shrinkScale;
	Vector3 m_growScale;
	bool m_growing = false;
	bool m_shrinking = false;
	float m_scaleMulti = 0;
	float m_growthDuration = 0;
	float m_shrinkDuration = 0;
	float m_sizeChangeDuration = 0;
	float m_sizeChangeTimeElapsed = 0;
	enum ScaleState { GROW, MAINTIAIN, SHRINK } m_scaleState = GROW;
};