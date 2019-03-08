#pragma once
#include "StepTimer.h"
#include "Constants.h"
#include "InputData.h"
#include "WindowData.h"

#include <vector>

class Scene
{

public:
	Scene() {};
	~Scene() {};

	virtual Scenes Update(GameStateData* _GSD, InputData* _ID) = 0;
	virtual void Render(RenderData* _RD, WindowData* _WD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) = 0;

	//Loads all the data needed for the current scene.
	virtual bool Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD) = 0;

protected:
	Scenes nextScene = Scenes::NONE;
};