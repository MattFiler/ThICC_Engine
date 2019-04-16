#pragma once
#include "StepTimer.h"
#include "Constants.h"
#include "InputData.h"

#include <vector>

class Scene
{

public:
	Scene() {};
	~Scene() {};

	virtual void Update() = 0;
	virtual void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) = 0;

	//Loads all the data needed for the current scene.
	virtual bool Load() = 0;
};