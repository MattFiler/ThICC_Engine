#pragma once
#include "StepTimer.h"
#include "Constants.h"
#include "InputData.h"

#include <vector>

class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	virtual void Update() = 0;
	virtual void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList) = 0;

	//Loads all the data needed for the current scene.
	virtual bool Load() = 0;

	//Performs an expensive load, bringing in models, etc ready for a scene to be rendered
	virtual void ExpensiveLoad() = 0;

	//Unloads memory for expensive objects in a scene if we're not being rendered
	virtual void ExpensiveUnload() = 0;

	//Create objects in the scene
	virtual void create2DObjects() = 0;
	virtual void create3DObjects() = 0;
	virtual void pushBackObjects() = 0;
};