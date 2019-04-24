#pragma once
#include "pch.h"
#include "StepTimer.h"

struct ThICC_GameStateData
{
	//length of time since last frame
	float m_dt;

	//Shared timer
	DX::StepTimer m_timer;

	//Player State Data
	int character_selected[4] = { 0,0,0,0 };
	int vehicle_selected[4] = { 0,0,0,0 };
};