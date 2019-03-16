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

void SceneManager::addScene(Scene* _scene, Scenes _scene_name) {
	_scene->Load(); //currently all scenes load when added (eww)
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
	m_scenes[int(m_curr_scene)]->Update();
}

void SceneManager::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList)
{
	m_scenes[int(m_curr_scene)]->Render(m_commandList);
}