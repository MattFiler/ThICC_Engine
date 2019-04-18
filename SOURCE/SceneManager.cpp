#include "pch.h"
#include "SceneManager.h"
#include "DeviceData.h"

/* Create */
SceneManager::SceneManager()
{
	m_scenes.reserve((int)Scenes::SCENE_COUNT);
}

/* Destroy */
SceneManager::~SceneManager() {
	//This, or just clear?
	for (Scene* this_scene : m_scenes) {
		delete this_scene;
	}
}

/* Add a scene to the manager*/
void SceneManager::addScene(Scene* _scene, Scenes _scene_name) {
	//Perform an inexpensive load of the scene's components and save it to our scene array
	_scene->Load();
	m_scenes.push_back(_scene);
	m_sceneDescriptors.push_back(_scene_name);
}

/* Set the current scene */
void SceneManager::setCurrentScene(Scenes _scene_name, bool _first_load) {
	//If we already have an unload queued we cannot change scene
	if (needs_delete) {
		DebugText::print("Requested a scene change, but it CANNOT happen! We're still queued up to remove the old scene. Try again later.");
		return;
	}

	//Queue up an unload of the old scene's expensive items if this isn't the first load
	if (!_first_load) {
		needs_delete = true;
		m_prev_scene = m_curr_scene;
	}

	//Set the current scene to render/update and load in our expensive stuff

	int index = 0;
	for (Scenes this_sceneDescriptor : m_sceneDescriptors) {
		if (this_sceneDescriptor == _scene_name) {
			m_curr_scene = m_scenes.at(index);
			m_curr_scene->ExpensiveLoad();
			return;
		}
		index++;
	}

	DebugText::print("FAILED to find the requested scene (" + std::to_string((int)_scene_name) + ").");
}

/* Update the current scene */
void SceneManager::Update() {
	//Keep a track of the scene unload queue
	if (needs_delete) {
		delete_counter++;
		if (delete_counter > 15) {
			//We can unload our scene, perform the unload
			Locator::getDD()->m_deviceResources->WaitForGpu();
			m_prev_scene->ExpensiveUnload();
			needs_delete = false;
			delete_counter = 0;
		}
	}

	//Update
	m_curr_scene->Update();
}

/* Render our active scene */
void SceneManager::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	m_curr_scene->Render(m_commandList);
}