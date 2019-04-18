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

	//Create objects in the scene
	virtual void create2DObjects();
	virtual void create3DObjects();
	virtual void pushBackObjects();

protected:
	//Structures for the scene's objects
	vector<GameObject2D*> m_2DObjects;
	vector<GameObject3D*> m_3DObjects;
	std::vector<PhysModel*> m_physModels;

	//Common engine components
	LocalisationManager m_localiser;
	KeybindManager m_keybinds;
	GameFilepaths m_filepath;
};