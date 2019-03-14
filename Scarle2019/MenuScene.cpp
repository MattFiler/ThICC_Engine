#include "pch.h"
#include "MenuScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();

MenuScene::MenuScene()
{
	m_scene_manager = new SceneManager();
}

MenuScene::~MenuScene()
{
	m_2DObjects.clear();
	m_3DObjects.clear();
}

void MenuScene::Update(GameStateData* _GSD, InputData* _ID)
{
	if (m_key.keyPressed("Quit"))
	{
		ExitGame();
	}
	else if (_GSD->m_keyboardState.Enter)
	{
		m_scene_manager->setCurrentScene(Scenes::GAMESCENE);
	}

	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick(_GSD);
	}

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Tick(_GSD);
	}
}

void MenuScene::Render(RenderData* _RD, WindowData* _WD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	m_commandList->RSSetViewports(1, &_WD->men_viewport);
	m_commandList->RSSetScissorRects(1, &_WD->men_scissorRect);
	_RD->m_cam = m_cam;

	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Render(_RD);
	}
}

bool MenuScene::Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	create2DObjects(_RD);
	create3DObjects(_RD, _ID, _WD);
	VBGO3D::PushIBVB(_RD); //DO NOT REMOVE THIS EVEN IF THERE ARE NO VBGO3Ds
	pushBackObjects(_RD);

	return true;
}

void MenuScene::create2DObjects(RenderData * _RD)
{
	//test text
	Text2D* m_enterMenu = new Text2D("Lewis is not cool.", _RD);
	m_2DObjects.push_back(m_enterMenu);
}

void MenuScene::create3DObjects(RenderData * _RD, InputData * _ID, WindowData * _WD)
{
	for (int i = 0; i < num_of_cam; i++) {
		//Create a player and position on track
		//player[i] = new Player(_RD, "Standard Kart", i, *_ID->m_gamePad.get());
		//player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		//m_3DObjects.push_back(player[i]);

		//Create a camera to follow the player
		m_cam = new Camera(_WD->m_outputWidth, _WD->m_outputHeight, 1.0f, 2000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
		m_cam->SetBehav(Camera::BEHAVIOUR::LERP);
		m_3DObjects.push_back(m_cam);

		//Create a viewport
		float width_mod = 0.5f;
		float height_mod = 0.5f;
		switch (i) {
		case 0: {
			*&_WD->men_viewport = { 0.0f, 0.0f, static_cast<float>(_WD->m_outputWidth), static_cast<float>(_WD->m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&_WD->men_scissorRect = { 0,0,(int)(_WD->m_outputWidth),(int)(_WD->m_outputHeight) };
			break;
		}
		//case 1: {
		//	_WD->m_viewport[i] = { static_cast<float>(*&_WD->m_outputWidth) * 0.5f, 0.0f, static_cast<float>(*&_WD->m_outputWidth)* 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
		//	_WD->m_scissorRect[i] = { 0,0,(int)(*&_WD->m_outputWidth),(int)(_WD->m_outputHeight * 0.5f) };
		//	break;
		//}
		//case 2: {
		//	*&_WD->m_viewport[i] = { 0.0f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, static_cast<float>(*&_WD->m_outputWidth) * 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
		//	*&_WD->m_scissorRect[i] = { 0,0,(int)(*&_WD->m_outputWidth * 0.5f),(int)(*&_WD->m_outputHeight) };
		//	break;
		//}
		//case 3: {
		//	*&_WD->m_viewport[i] = { static_cast<float>(*&_WD->m_outputWidth) * 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, static_cast<float>(*&_WD->m_outputWidth) * 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
		//	*&_WD->m_scissorRect[i] = { 0,0,(int)(*&_WD->m_outputWidth),(int)(*&_WD->m_outputHeight) };
		//	break;
		//}
		}
	}
}

void MenuScene::pushBackObjects(RenderData * _RD)
{
	for (int i = 0; i < m_3DObjects.size(); i++)
	{
		if (dynamic_cast<PhysModel*>(m_3DObjects[i]) && dynamic_cast<PhysModel*>(m_3DObjects[i])->hasCollider())
		{
			m_physModels.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i]));
			m_3DObjects.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i])->getDebugCollider());
		}
	}
}
