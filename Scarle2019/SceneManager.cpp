#include "pch.h"
#include "SceneManager.h"
#include "ServiceLocator.h"
#include "GarbageCollector.h"


Scenes SceneManager::m_curr_scene = Scenes::MENUSCENE;

SceneManager::SceneManager()
{
	//m_scenes = new Scene*[Scenes::SCENE_COUNT];
	m_current_scene = new MenuScene();
	m_current_scene->Load();
}

SceneManager::~SceneManager() {
	delete m_scenes;
	m_scenes = nullptr;
	Locator::getGarbageCollector()->DeletePointer(m_current_scene);
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

void SceneManager::settCurrentScene(Scenes _scene) {
	//Delete old scene here
	switch (_scene)
	{
	case Scenes::MENUSCENE:

		m_current_scene = new MenuScene();
		break;
	case Scenes::GAMESCENE:
		m_current_scene = new GameScene();
		break;
	}

	//Then load new scene
}

Scenes SceneManager::getCurrentScene() {
	return m_curr_scene;
};

void SceneManager::Update() {
	//m_scenes[int(m_curr_scene)]->Update();
	switch (m_current_scene->Update())
	{
	case Scenes::MENUSCENE:
		Locator::getGarbageCollector()->DeletePointer(m_current_scene);
		m_current_scene = new MenuScene();
		m_current_scene->Load();
		break;
	case Scenes::GAMESCENE:
		Locator::getGarbageCollector()->DeletePointer(m_current_scene);
		m_current_scene = new GameScene();
		m_current_scene->Load();
		break;
	}
}

void SceneManager::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList)
{
	//m_scenes[int(m_curr_scene)]->Render(m_commandList);
	m_current_scene->Render(m_commandList);
}