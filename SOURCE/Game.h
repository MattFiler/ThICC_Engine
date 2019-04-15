#pragma once

#include "StepTimer.h"

class ThICC_Game {
public:
	ThICC_Game() = default;
	~ThICC_Game() = default;

	void Update(DX::StepTimer const& timer);
	void Render();
};