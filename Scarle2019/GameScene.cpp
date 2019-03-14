#include "pch.h"
#include "GameScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include <iostream>
#include <experimental/filesystem>

extern void ExitGame();


GameScene::GameScene()
{
	m_scene_manager = new SceneManager();
}


GameScene::~GameScene()
{
	m_2DObjects.clear();
	m_3DObjects.clear();
}

void GameScene::Update(GameStateData* _GSD, InputData* _ID)
{
	for (int i = 0; i < game_config["player_count"]; ++i) {
		player[i]->ShouldStickToTrack(*track, _GSD);
		player[i]->ResolveWallCollisions(*track);
		_GSD->m_gamePadState[i] = _ID->m_gamePad->GetState(i); //set game controllers state[s]
	}

	if (m_keybinds.keyPressed("Quit"))
	{
		m_scene_manager->setCurrentScene(Scenes::MENUSCENE);
	}
	if (m_keybinds.keyPressed("Orbit"))
	{
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::ORBIT);
	}
	if (m_keybinds.keyPressed("Lerp"))
	{
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::LERP);
	}
	if (m_keybinds.keyPressed("Matt"))
	{
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::MATT_CAM);
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

void GameScene::Render(RenderData* _RD, WindowData* _WD, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	//draw 3D objects

	//camera setup.
	for (int i = 0; i < game_config["player_count"]; ++i)
	{
		m_commandList->RSSetViewports(1, &_WD->m_viewport[i]);
		m_commandList->RSSetScissorRects(1, &_WD->m_scissorRect[i]);
		_RD->m_cam = m_cam[i];

		for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
		{
			(*it)->Render(_RD);
		}
	}

	ID3D12DescriptorHeap* heaps[] = { _RD->m_resourceDescriptors->Heap() };
	m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	m_commandList->RSSetViewports(1, &_WD->sprite_viewport);
	m_commandList->RSSetScissorRects(1, &_WD->sprite_rect);
	_RD->m_spriteBatch->SetViewport(_WD->sprite_viewport);
	_RD->m_spriteBatch->Begin(m_commandList.Get());

	//draw 2d objects
	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Render(_RD);
	}
	_RD->m_spriteBatch->End();
}

bool GameScene::Load(GameStateData* _GSD, RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	//Read in track config
	std::ifstream i(m_filepath.generateFilepath("GAME_CORE", m_filepath.CONFIG));
	game_config << i;



	create3DObjects(_RD ,_ID, _WD);

	create2DObjects(_RD, _WD);

	VBGO3D::PushIBVB(_RD); //DO NOT REMOVE THIS EVEN IF THERE ARE NO VBGO3Ds

	pushBackObjects(_RD);

	return true;
}

void GameScene::create2DObjects(RenderData* _RD, WindowData* _WD)
{
	//test text
	//Text2D *test2 = new Text2D(m_localiser.getString("debug_text"));
	//m_2DObjects.push_back(test2);
	ImageGO2D *test[4]; 
	Text2D *text[4];
	for (int i = 0; i < game_config["player_count"]; i++)
	{
		test[i]= new ImageGO2D(_RD, "twist");
		//test->SetOri(45);
		test[i]->SetScale(0.1f*Vector2::One);
		test[i]->SetPos(Vector2(_WD->m_viewport[i].TopLeftX, _WD->m_viewport[i].TopLeftY));
		//test->CentreOrigin();
		m_2DObjects.push_back(test[i]);
		text[i] = new Text2D(m_localiser.getString("debug_text"), _RD);

		float text_pos_x = _WD->m_viewport[i].TopLeftX + _WD->m_viewport[i].Width - text[i]->GetSize().x;
		float text_pos_y = _WD->m_viewport[i].TopLeftY + _WD->m_viewport[i].Height - text[i]->GetSize().y;
		text[i]->SetPos(Vector2(text_pos_x, text_pos_y));
		m_2DObjects.push_back(text[i]);
	}

	*&_WD->sprite_viewport = { 0.0f, 0.0f, static_cast<float>(_WD->m_outputWidth), static_cast<float>(_WD->m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	*&_WD->sprite_rect = { 0,0,(int)(_WD->m_outputWidth),(int)(_WD->m_outputHeight) };
}

void GameScene::create3DObjects(RenderData* _RD, InputData* _ID, WindowData* _WD)
{
	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	for (int i = 1; i < GP_COUNT; i++)
	{
		//GPGO3D* test3d2 = new GPGO3D((GPGO3D_Type)i);
		//test3d2->SetPos(12.0f*Vector3::Forward + 10.0f*(i - 1)*Vector3::Left);
		//test3d2->SetScale(5.0f);
		//m_3DObjects.push_back(test3d2);
	}

	//Load in a track
	track = new Track(_RD, game_config["default_track"]);
	m_3DObjects.push_back(track);

	Vector3 suitable_spawn = track->getSuitableSpawnSpot();
	for (int i = 0; i < game_config["player_count"]; i++) {
		//Create a player and position on track
		player[i] = new Player(_RD, "Standard Kart", i, *_ID->m_gamePad.get());
		player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		m_3DObjects.push_back(player[i]);

		//Create a camera to follow the player
		m_cam[i] = new Camera(_WD->m_outputWidth, _WD->m_outputHeight, 1.0f, 2000.0f, player[i], Vector3(0.0f, 3.0f, 10.0f));
		m_cam[i]->SetBehav(Camera::BEHAVIOUR::LERP);
		m_3DObjects.push_back(m_cam[i]);

		//Create a viewport
		float width_mod = 0.5f;
		float height_mod = 0.5f;
		switch (i) {
		case 0: {
			*&_WD->m_viewport[i] = { 0.0f, 0.0f, static_cast<float>(_WD->m_outputWidth) * 0.5f, static_cast<float>(_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&_WD->m_scissorRect[i] = { 0,0,(int)(_WD->m_outputWidth * 0.5f),(int)(_WD->m_outputHeight * 0.5f) };
			break;
		}
		case 1: {
			*&_WD->m_viewport[i] = { static_cast<float>(_WD->m_outputWidth) * 0.5f, 0.0f, static_cast<float>(_WD->m_outputWidth)* 0.5f, static_cast<float>(_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&_WD->m_scissorRect[i] = { 0,0,(int)(_WD->m_outputWidth),(int)(_WD->m_outputHeight * 0.5f) };
			break;
		}
		case 2: {
			*&_WD->m_viewport[i] = { 0.0f, static_cast<float>(_WD->m_outputHeight) * 0.5f, static_cast<float>(_WD->m_outputWidth) * 0.5f, static_cast<float>(_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&_WD->m_scissorRect[i] = { 0,0,(int)(_WD->m_outputWidth * 0.5f),(int)(_WD->m_outputHeight) };
			break;
		}
		case 3: {
			*&_WD->m_viewport[i] = { static_cast<float>(_WD->m_outputWidth) * 0.5f, static_cast<float>(_WD->m_outputHeight) * 0.5f, static_cast<float>(_WD->m_outputWidth) * 0.5f, static_cast<float>(*&_WD->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&_WD->m_scissorRect[i] = { 0,0,(int)(_WD->m_outputWidth),(int)(_WD->m_outputHeight) };
			break;
		}
		}
	}

	//Global illumination
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_3DObjects.push_back(m_light);
	_RD->m_light = m_light;
}

void GameScene::pushBackObjects(RenderData* _RD)
{
	//Add all 3D game objects with a collider to the collision manager's list
	for (int i = 0; i < m_3DObjects.size(); i++)
	{
		if (dynamic_cast<PhysModel*>(m_3DObjects[i]) && dynamic_cast<PhysModel*>(m_3DObjects[i])->hasCollider())
		{
			m_physModels.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i]));
			m_3DObjects.push_back(dynamic_cast<PhysModel*>(m_3DObjects[i])->getDebugCollider());
		}
	}
}
