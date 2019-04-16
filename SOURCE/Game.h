#pragma once

#include "StepTimer.h"

class ThICC_Game {
public:
	ThICC_Game() = default;
	~ThICC_Game() = default;

	void Update(DX::StepTimer const& timer);
	void InputHandler(DirectX::GamePad::State gpad);
	void InputHandler(DirectX::Keyboard::State kb, DirectX::Mouse::State mouse);
	void Render();
};