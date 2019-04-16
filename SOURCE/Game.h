#pragma once

#include "StepTimer.h"

class ThICC_Game {
public:
	ThICC_Game() = default;
	~ThICC_Game() = default;

	void Initialize();
	void Update(DX::StepTimer const& timer);
	void Render();
};