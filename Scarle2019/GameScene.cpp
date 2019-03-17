#include "pch.h"
#include "GameScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "GameDebugToggles.h"
#include "ServiceLocator.h"
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

void GameScene::Update()
{
	for (int i = 0; i < game_config["player_count"]; ++i) {
		player[i]->ShouldStickToTrack(*track);
		player[i]->ResolveWallCollisions(*track);
		Locator::getGSD()->m_gamePadState[i] = Locator::getID()->m_gamePad->GetState(i); //set game controllers state[s]
	}

	UpdateItems();

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
		if (m_cam[0]->GetBehav() == Camera::BEHAVIOUR::DEBUG_CAM) {
			m_cam[0]->SetBehav(Camera::BEHAVIOUR::LERP);
			return;
		}
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::DEBUG_CAM);
	}

	for (int i = 0; i < game_config["player_count"]; i++) {
		for (size_t j = 0; j < track->getWaypointsBB().size(); j++)
		{
			if (player[i]->getCollider().Intersects(track->getWaypointsBB()[j]))
			{
				player[i]->setCurrentWaypoint(j);
			}
		}
	}

	for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
	{
		(*it)->Tick();
	}

	// Note I've changed this to a loop via index since Tick() on players
	// can introduce new objects, which causes some horrible errors
	int end = m_3DObjects.size();
	int delIndex = -1;
	for (int i = 0; i < end; i++)
	{
		m_3DObjects[i]->Tick();
		if (m_3DObjects[i]->ShouldDestroy())
		{
			delIndex = i;
		}
	}
	if (delIndex != -1)
	{
		//delete m_3DObjects[delIndex];
		m_3DObjects.erase(m_3DObjects.begin() + delIndex);
	}

	//Toggle debug mesh renders
	if (m_keybinds.keyPressed("Debug Toggle"))
	{
		GameDebugToggles::show_debug_meshes = !GameDebugToggles::show_debug_meshes;
	}

	CollisionManager::collisionDetectionAndResponse(m_physModels);
}

void GameScene::UpdateItems()
{
	int delIndex = -1;
	int end = m_itemModels.size();
	for (int i = 0; i < end; i++)
	{
		m_itemModels[i]->ShouldStickToTrack(*track);
		m_itemModels[i]->ResolveWallCollisions(*track);
		for (int j = 0; j < game_config["player_count"]; ++j) {
			if (player[j]->getCollider().Intersects(m_itemModels[i]->getCollider()))
			{
				m_itemModels[i]->HitByPlayer(player[j]);
			}
		}
		m_itemModels[i]->Tick();
		if (m_itemModels[i]->ShouldDestroy())
		{
			delIndex = i;
		}
	}
	if (delIndex != -1)
	{
		//delete m_itemModels[delIndex];
		m_itemModels.erase(m_itemModels.begin() + delIndex);
	}
}

void GameScene::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	//draw 3D objects

	//camera setup.
	for (int i = 0; i < game_config["player_count"]; ++i)
	{
		m_commandList->RSSetViewports(1, &Locator::getWD()->m_viewport[i]);
		m_commandList->RSSetScissorRects(1, &Locator::getWD()->m_scissorRect[i]);
		Locator::getRD()->m_cam = m_cam[i];

		for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
		{
			(*it)->Render();
		}

		for (GameObject3D* obj : m_itemModels)
		{
			obj->Render();
		}
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

bool GameScene::Load()
{
	//Read in track config
	std::ifstream i(m_filepath.generateFilepath("GAME_CORE", m_filepath.CONFIG));
	game_config << i;

	create3DObjects();

	create2DObjects();

	VBGO3D::PushIBVB(); //DO NOT REMOVE THIS EVEN IF THERE ARE NO VBGO3Ds

	pushBackObjects();

	return true;
}

void GameScene::create2DObjects()
{
	//test text
	//Text2D *test2 = new Text2D(m_localiser.getString("debug_text"));
	//m_2DObjects.push_back(test2);
	ImageGO2D *test[4]; 
	Text2D *text[4];
	for (int i = 0; i < game_config["player_count"]; i++)
	{
		test[i] = new ImageGO2D("twist");
		//test->SetOri(45);
		test[i]->SetScale(0.1f*Vector2::One);
		test[i]->SetPos(Vector2(Locator::getWD()->m_viewport[i].TopLeftX, Locator::getWD()->m_viewport[i].TopLeftY));
		//test->CentreOrigin();
		m_2DObjects.push_back(test[i]);

		//player[i] = new Text2D(m_localiser.getString(std::to_string(player[i]->getCurrentWaypoint())), _RD);
		float text_pos_x = Locator::getWD()->m_viewport[i].TopLeftX + Locator::getWD()->m_viewport[i].Width - player[i]->getPosition()->GetSize().x;
		float text_pos_y = Locator::getWD()->m_viewport[i].TopLeftY + Locator::getWD()->m_viewport[i].Height - player[i]->getPosition()->GetSize().y;
		player[i]->getPosition()->SetPos(Vector2(text_pos_x, text_pos_y));
		m_2DObjects.push_back(player[i]->getPosition());
	}

	*&Locator::getWD()->sprite_viewport = { 0.0f, 0.0f, static_cast<float>(Locator::getWD()->m_outputWidth), static_cast<float>(Locator::getWD()->m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	*&Locator::getWD()->sprite_rect = { 0,0,(int)(Locator::getWD()->m_outputWidth),(int)(Locator::getWD()->m_outputHeight) };
}

void GameScene::create3DObjects()
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
	track = new Track(game_config["default_track"]);
	track->setUpWaypointBB();
	m_3DObjects.push_back(track);

	Vector3 suitable_spawn = track->getSuitableSpawnSpot();
	for (int i = 0; i < game_config["player_count"]; i++) {
		//Create a player and position on track
		using std::placeholders::_1;
		player[i] = new Player("Knuckles Kart", i, std::bind(&GameScene::CreateItem, this, _1));
		player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		m_3DObjects.push_back(player[i]);

		//Create a camera to follow the player
		m_cam[i] = new Camera(Locator::getWD()->m_outputWidth, Locator::getWD()->m_outputHeight, 1.0f, 2000.0f, player[i], Vector3(0.0f, 3.0f, 10.0f));
		m_cam[i]->SetBehav(Camera::BEHAVIOUR::LERP);
		m_3DObjects.push_back(m_cam[i]);

		//Create a viewport
		float width_mod = 0.5f;
		float height_mod = 0.5f;
		switch (i) {
		case 0: {
			*&Locator::getWD()->m_viewport[i] = { 0.0f, 0.0f, static_cast<float>(Locator::getWD()->m_outputWidth) * 0.5f, static_cast<float>(Locator::getWD()->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&Locator::getWD()->m_scissorRect[i] = { 0,0,(int)(Locator::getWD()->m_outputWidth * 0.5f),(int)(Locator::getWD()->m_outputHeight * 0.5f) };
			break;
		}
		case 1: {
			*&Locator::getWD()->m_viewport[i] = { static_cast<float>(Locator::getWD()->m_outputWidth) * 0.5f, 0.0f, static_cast<float>(Locator::getWD()->m_outputWidth)* 0.5f, static_cast<float>(Locator::getWD()->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&Locator::getWD()->m_scissorRect[i] = { 0,0,(int)(Locator::getWD()->m_outputWidth),(int)(Locator::getWD()->m_outputHeight * 0.5f) };
			break;
		}
		case 2: {
			*&Locator::getWD()->m_viewport[i] = { 0.0f, static_cast<float>(Locator::getWD()->m_outputHeight) * 0.5f, static_cast<float>(Locator::getWD()->m_outputWidth) * 0.5f, static_cast<float>(Locator::getWD()->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&Locator::getWD()->m_scissorRect[i] = { 0,0,(int)(Locator::getWD()->m_outputWidth * 0.5f),(int)(Locator::getWD()->m_outputHeight) };
			break;
		}
		case 3: {
			*&Locator::getWD()->m_viewport[i] = { static_cast<float>(Locator::getWD()->m_outputWidth) * 0.5f, static_cast<float>(Locator::getWD()->m_outputHeight) * 0.5f, static_cast<float>(Locator::getWD()->m_outputWidth) * 0.5f, static_cast<float>(*&Locator::getWD()->m_outputHeight) * 0.5f, D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
			*&Locator::getWD()->m_scissorRect[i] = { 0,0,(int)(Locator::getWD()->m_outputWidth),(int)(Locator::getWD()->m_outputHeight) };
			break;
		}
		}
	}

	//Global illumination
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_3DObjects.push_back(m_light);
	Locator::getRD()->m_light = m_light;
}

void GameScene::pushBackObjects()
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

Item* GameScene::CreateItem(ItemType type)
{
	switch (type)
	{
	case BANANA:
	{
		Banana* banana = new Banana();
		m_itemModels.push_back(banana);
		return banana;
		break;
	}
	case GREEN_SHELL:
	{
		GreenShell* greenShell = new GreenShell();
		m_itemModels.push_back(greenShell);
		return greenShell;
		break;
	}
	case RED_SHELL:
		break;
	case MUSHROOM:
		break;
	default:
		return nullptr;
		break;
	}
	return nullptr;
}
