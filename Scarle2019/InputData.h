#pragma once
#include "pch.h"

using namespace DirectX;

struct InputData
{
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::GamePad> m_gamePad;
};