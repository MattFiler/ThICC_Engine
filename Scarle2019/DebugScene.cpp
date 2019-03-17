#include "pch.h"
#include "DebugScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "GameDebugToggles.h"
#include "ServiceLocator.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();


DebugScene::DebugScene()
{
	m_scene_manager = new SceneManager();
}


DebugScene::~DebugScene()
{
	m_2DObjects.clear();
	m_3DObjects.clear();
}

void DebugScene::Update()
{
	Locator::getGSD()->m_gamePadState[0] = Locator::getID()->m_gamePad->GetState(0); //set game controllers state[s]

	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick();
	}

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Tick();
	}
}

void DebugScene::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList)
{
	//draw 3D objects

	//camera setup.
	m_commandList->RSSetViewports(1, &Locator::getWD()->m_viewport[0]);
	m_commandList->RSSetScissorRects(1, &Locator::getWD()->m_scissorRect[0]);
	Locator::getRD()->m_cam = m_cam;

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Render();
	}

	ID3D12DescriptorHeap* heaps[] = { Locator::getRD()->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	m_commandList->RSSetViewports(1, &Locator::getWD()->sprite_viewport);
	m_commandList->RSSetScissorRects(1, &Locator::getWD()->sprite_rect);
	Locator::getRD()->m_spriteBatch->SetViewport(Locator::getWD()->sprite_viewport);
	Locator::getRD()->m_spriteBatch->Begin(m_commandList.Get());

	//draw 2d objects
	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Render();
	}
	Locator::getRD()->m_spriteBatch->End();
}

bool DebugScene::Load()
{
	Text2D *debug_text = new Text2D("DEBUG SCENE");
	m_2DObjects.push_back(debug_text);

	//Create a camera
	m_cam = new Camera(Locator::getWD()->m_outputWidth, Locator::getWD()->m_outputHeight, 1.0f, 2000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	m_cam->SetBehav(Camera::BEHAVIOUR::DEBUG_CAM);
	m_cam->SetPos(Vector3(0, 50, 0));
	m_3DObjects.push_back(m_cam);
	*&Locator::getWD()->m_viewport[0] = { 0.0f, 0.0f, static_cast<float>(Locator::getWD()->m_outputWidth), static_cast<float>(Locator::getWD()->m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	*&Locator::getWD()->m_scissorRect[0] = { 0,0,(int)(Locator::getWD()->m_outputWidth),(int)(Locator::getWD()->m_outputHeight) };

	//Load in debug objects
	test_model = new SDKMeshGO3D("TEST_ILLUM");
	m_3DObjects.push_back(test_model);

	//Global illumination
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_3DObjects.push_back(m_light);
	Locator::getRD()->m_light = m_light;

	VBGO3D::PushIBVB(); //DO NOT REMOVE THIS EVEN IF THERE ARE NO VBGO3Ds

	//Add all 3D game objects with a collider to the collision manager's list
	for (int i = 0; i < m_3DObjects.size(); i++)
	{
		if (dynamic_cast<PhysModel*>(m_3DObjects[i]) && dynamic_cast<PhysModel*>(m_3DObjects[i])->hasCollider())
		{
			m_3DObjects.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i])->getDebugCollider());
		}
	}

	return true;
}
