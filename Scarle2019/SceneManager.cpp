#include "pch.h"
#include "SceneManager.h"

Scenes SceneManager::m_curr_scene = Scenes::MENUSCENE;

SceneManager::SceneManager()
{
	m_scenes = new Scene*[Scenes::SCENE_COUNT];
}

SceneManager::~SceneManager() {
	delete m_scenes;
	m_scenes = nullptr;
}

void SceneManager::configure(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD) {
	m_RD = _RD;
	m_GSD = _GSD;
	m_ID = _ID;
	m_WD = _WD;
}

void SceneManager::addScene(Scene* _scene, Scenes _scene_name) {
	_scene->Load(m_GSD, m_RD, m_ID, m_WD); //currently all scenes load when added (eww)
	m_scenes[int(_scene_name)] = _scene;
}

void SceneManager::setCurrentScene(Scenes _scene_name) {
	//Delete old scene here
	m_curr_scene = _scene_name;
	//Then load new scene
}
Scenes SceneManager::getCurrentScene() {
	return m_curr_scene;
};

void SceneManager::Update() {
	m_scenes[int(m_curr_scene)]->Update(m_GSD, m_ID);
}

void SceneManager::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	m_scenes[int(m_curr_scene)]->Render(m_RD, m_WD, m_commandList);
}