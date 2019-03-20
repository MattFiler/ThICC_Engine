#pragma once
#include "pch.h"

//GEP:: Basic data of current GameState to communicate to all Game Objects

struct GameStateData
{
	//length of time since last frame
	float m_dt;

	//input device states
	DirectX::Keyboard::State m_prevKeyboardState;
	DirectX::Keyboard::State m_keyboardState;
	DirectX::Mouse::State m_mouseState;
	DirectX::GamePad::State m_gamePadState[4];

	//Player State Data
	int charecter_selected[4] = {0,0,0,0};

};

