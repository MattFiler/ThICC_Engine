#include "pch.h"
#include "SceneManager.h"
#include "WaitForGPU.h"

#include "MenuScene.h"
#include "GameScene.h"
#include "DebugScene.h"

Scenes SceneManager::m_curr_scene = Scenes::SCENE_COUNT;
Scenes SceneManager::m_new_scene = Scenes::MENUSCENE;

SceneManager::SceneManager()
{
	//m_scenes = new Scene*[Scenes::SCENE_COUNT];
}

SceneManager::~SceneManager() {
	delete m_scene_mem_1;
	m_scene_mem_1 = nullptr;
	delete m_scene_mem_2;
	m_scene_mem_2 = nullptr;
}

void SceneManager::addScene(Scene* _scene, Scenes _scene_name) {
	//nothing for now
}

void SceneManager::setCurrentScene(Scenes _scene_name) {
	m_new_scene = _scene_name;
}
Scenes SceneManager::getCurrentScene() {
	return m_curr_scene;
};

void SceneManager::Update() {
	if (m_new_scene == m_curr_scene) {
		if (m_scene_mem_1 != nullptr && is_safe_1) {
			m_scene_mem_1->Update();
		}
		else if (m_scene_mem_2 != nullptr && is_safe_2) {
			m_scene_mem_2->Update();
		}
		else
		{
			//throw std::runtime_error("Ran out of scene memory space!");
		}
	}
}

void SceneManager::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList)
{
	is_safe_1 = false;
	is_safe_2 = false;

	if (m_new_scene != m_curr_scene) {
		switch (m_new_scene) {
			case Scenes::MENUSCENE: {
				if (m_scene_mem_1 == nullptr) {
					m_scene_mem_1 = new MenuScene();
					m_scene_mem_1->Load();
					did_just_update_1 = true;
					WaitForGPU::should_wait = true;
					m_curr_scene = m_new_scene;
				}
				else if (m_scene_mem_2 == nullptr) {
					m_scene_mem_2 = new MenuScene();
					m_scene_mem_2->Load();
					did_just_update_2 = true;
					WaitForGPU::should_wait = true;
					m_curr_scene = m_new_scene;
				}
				else
				{
					throw std::runtime_error("Ran out of scene memory space!");
				}
				break;
			}
			case Scenes::GAMESCENE: {
				if (m_scene_mem_1 == nullptr) {
					m_scene_mem_1 = new GameScene();
					m_scene_mem_1->Load();
					did_just_update_1 = true;
					WaitForGPU::should_wait = true;
					m_curr_scene = m_new_scene;
				}
				else if (m_scene_mem_2 == nullptr) {
					m_scene_mem_2 = new GameScene();
					m_scene_mem_2->Load();
					did_just_update_2 = true;
					WaitForGPU::should_wait = true;
					m_curr_scene = m_new_scene;
				}
				else
				{
					throw std::runtime_error("Ran out of scene memory space!");
				}
				break;
			}
			case Scenes::DEBUGSCENE: {
				if (m_scene_mem_1 == nullptr) {
					m_scene_mem_1 = new DebugScene();
					m_scene_mem_1->Load();
					did_just_update_1 = true;
					WaitForGPU::should_wait = true;
					m_curr_scene = m_new_scene;
				}
				else if (m_scene_mem_2 == nullptr) {
					m_scene_mem_2 = new DebugScene();
					m_scene_mem_2->Load();
					did_just_update_2 = true;
					WaitForGPU::should_wait = true;
					m_curr_scene = m_new_scene;
				}
				else
				{
					throw std::runtime_error("Ran out of scene memory space!");
				}
				break;
			}
		}
	}
	else
	{
		if (did_just_update_1) {
			if (m_scene_mem_2 != nullptr) {
				delete m_scene_mem_2;
				m_scene_mem_2 = nullptr;
			}
			did_just_update_1 = false;
			WaitForGPU::should_wait = true;
		}
		else if (did_just_update_2) {
			if (m_scene_mem_1 != nullptr) {
				delete m_scene_mem_1;
				m_scene_mem_1 = nullptr;
			}
			did_just_update_2 = false;
			WaitForGPU::should_wait = true;
		}
		else
		{
			if (m_scene_mem_1 != nullptr) {
				is_safe_1 = true;
				m_scene_mem_1->Render(m_commandList);
			}
			else if (m_scene_mem_2 != nullptr) {
				is_safe_2 = true;
				m_scene_mem_2->Render(m_commandList);
			}
			else
			{
				throw std::runtime_error("Ran out of scene memory space!");
			}
		}
	}
}