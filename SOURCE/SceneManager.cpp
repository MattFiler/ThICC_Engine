#include "pch.h"
#include "SceneManager.h"
#include "DeviceData.h"

/* Create */
SceneManager::SceneManager() {
	m_scenes.reserve((int)Scenes::SCENE_COUNT);
}

/* Destroy */
SceneManager::~SceneManager() {
	for (Scene* this_scene : m_scenes) {
		delete this_scene;
	}
}

/* Create loadscreen sprite */
void SceneManager::Initialize() {
	loadscreen = new ImageGO2D("LOADING"); //It'd be nice to have some kind of generic map-specific loadscreen ability
}

/* Add a scene to the manager*/
void SceneManager::addScene(Scene* _scene, int _scene_name) {
	//Perform an inexpensive load of the scene's components and save it to our scene array
	_scene->Load();
	m_scenes.push_back(_scene);
	m_sceneDescriptors.push_back(_scene_name);
}

/* Set the current scene */
void SceneManager::setCurrentScene(int _scene_name, bool _first_load) {
	//If we already have an unload queued we cannot change scene
	if (needs_delete) {
		DebugText::print("Requested a scene change, but it CANNOT happen! We're still queued up to remove the old scene. Try again later.");
		return;
	}

	DebugText::print("SceneManager: Queued a change to scene " + std::to_string(_scene_name) + ".");

	//Queue up the switch (this is done purely for the ability to show a loadscreen)
	first_load = _first_load;
	scene_to_switch = _scene_name;
	switched_scene = false;
	if (first_load) {
		//Switch straight away if first load (we're pre-update)
		scene_switch = true;
	}
	else
	{
		//Queue up an unload of the old scene's expensive items if this isn't the first load
		needs_delete = true;
		m_prev_scene = m_curr_scene;
		show_loadscreen = true;
	}
}

/* Update the current scene */
void SceneManager::Update(DX::StepTimer const& timer) {
	//Set the current scene to render/update and load in our expensive stuff
	if (scene_switch) {
		int index = 0;
		for (int this_sceneDescriptor : m_sceneDescriptors) {
			if (this_sceneDescriptor == scene_to_switch) {
				DebugText::print("SceneManager: Changing scenes!");
				m_curr_scene = m_scenes.at(index);
				m_curr_scene->ExpensiveLoad();
				break;
			}
			index++;
		}
		scene_switch = false;
		switched_scene = true;
	}

	//Keep a track of the scene unload queue
	if (needs_delete) {
		show_loadscreen = false;
		delete_counter++;
		if (delete_counter > 15) {
			//We can unload our scene, perform the unload
			DebugText::print("SceneManager: Unloading previous scene!");
			Locator::getDD()->m_deviceResources->WaitForGpu();
			m_prev_scene->ExpensiveUnload();
			needs_delete = false;
			delete_counter = 0;
			/*
			It'd be real nice to ACTUALLY delete the scene here, 
			but doing so gives us a GXDI_DEVICE_REMOVED error which I can't solve.
			So, this is the solution!
			*/
		}
	}

	//Update
	m_curr_scene->Update(timer);
}

/* Render our active scene 3D */
void SceneManager::Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	m_curr_scene->Render3D(m_commandList);
}

/* Render our active scene 2D */
void SceneManager::Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	//Set to 2D viewport
	m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
	m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);

	//Set to 2D heaps
	ID3D12DescriptorHeap* heaps[] = { Locator::getRD()->m_2dResourceDescriptors->Heap() };
	Locator::getRD()->m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	//Start sprite batch and call 2D renders
	Locator::getRD()->m_2dSpriteBatch->Begin(Locator::getRD()->m_commandList.Get());
	m_curr_scene->Render2D(m_commandList);
	if (show_loadscreen) { 
		loadscreen->Render(); 
		if (!first_load && !switched_scene) {
			scene_switch = true;
		}
	}
	Locator::getRD()->m_2dSpriteBatch->End();
}