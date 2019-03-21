#pragma once

#include "Scene.h"
#include <vector>

using std::vector;

//predefs
class GameObject2D;
class GameObject3D;
struct RenderData;
struct InputData;
class SceneManager;

class DebugScene : public Scene
{
public:
	DebugScene();
	~DebugScene();

	void Update() override;
	void Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList) override;
	bool Load() override;

private:
	Player* player = nullptr;
	SDKMeshGO3D* test_model = nullptr;
	Camera* m_cam = nullptr;
	Light* m_light = nullptr;
	
	Track* m_track = nullptr;

	vector<GameObject2D*> m_2DObjects; 
	vector<GameObject3D*> m_3DObjects; 

	SceneManager* m_scene_manager;
};

