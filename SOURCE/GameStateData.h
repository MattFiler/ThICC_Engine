#pragma once
#include "pch.h"

struct ThICC_GameStateData
{
	//length of time since last frame
	float m_dt;

	//Player State Data
	int character_selected[4] = { 0,0,0,0 };
	int vehicle_selected[4] = { 0,0,0,0 };

};