#include "pch.h"
#include "GameScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "GameDebugToggles.h"
#include "ServiceLocator.h"
#include "DebugMarker.h"
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
	//  camera_pos->SetText(std::to_string((int)m_cam[0]->GetPos().x) + "," + std::to_string((int)m_cam[0]->GetPos().y) + "," + std::to_string((int)m_cam[0]->GetPos().z) + "\n" +
	//	std::to_string((int)m_cam[0]->GetOri().Translation().x) + "," + std::to_string((int)m_cam[0]->GetOri().Translation().y) + "," + std::to_string((int)m_cam[0]->GetOri().Translation().z));


	switch (state)
	{
	case OPENING:
		if (timeout > 0)
		{
			timeout -= Locator::getGSD()->m_dt;
		}
		else
		{
			state = COUNTDOWN;
			timeout = 2.99999f;
		}
		break;
	case COUNTDOWN:
		if (timeout > 0)
		{
			timeout -= Locator::getGSD()->m_dt;
			countdown_text->SetText(std::to_string((int)std::ceil(timeout)));
		}
		else
		{
			state = PLAY;
			for (int i = 0; i < 4; ++i)
			{
				player[i]->setGamePad(m_playerControls);
			}
		}
		break;
	}


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


	// sets the players waypoint
	SetPlayersWaypoint();

	// Sets the current position of the player on teh leaderboard
	SetPlayerRanking();

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
	if (m_keybinds.keyPressed("Debug Toggle World Render")) {
		GameDebugToggles::render_level = !GameDebugToggles::render_level;
	}
	else if (m_keybinds.keyPressed("Activate"))
	{
		state = PLAY;
		for (int i = 0; i < 4; ++i)
		{
			player[i]->setGamePad(m_playerControls);
		}
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

void GameScene::SetPlayersWaypoint()
{
	for (int i = 0; i < game_config["player_count"]; i++) {

		if (player[i]->GetWaypoint() < track->getWaypoints().size() -1)
		{
			if (player[i]->getCollider().Intersects(track->getWaypointsBB()[player[i]->GetWaypoint() + 1]))
			{
				player[i]->SetWaypoint(player[i]->GetWaypoint() + 1);
			}
		}
		else
		{
			if (player[i]->getCollider().Intersects(track->getWaypointsBB()[0]))
			{
				player[i]->SetWaypoint(0);
				player[i]->SetLap(player[i]->GetLap() + 1);
			}
		}
	}
}

void GameScene::SetPlayerRanking()
{
	for (int i = 0; i < game_config["player_count"]; i++)
	{
		player[i]->SetRanking(1);
	}

	for (int i = 0; i < game_config["player_count"]; i++)
	{
		for (int j = 0; j < game_config["player_count"]; j++)
		{
			if (i != j)
			{
				if (player[i]->GetLap() < player[j]->GetLap())
				{
					player[i]->SetRanking(player[i]->GetRanking() + 1);
				}
				else if (player[i]->GetLap() == player[j]->GetLap())
				{
					if (player[i]->GetWaypoint() < player[j]->GetWaypoint())
					{
						player[i]->SetRanking(player[i]->GetRanking() + 1);
					}
					else if (player[i]->GetWaypoint() == player[j]->GetWaypoint())
					{
						float difference = 0;
						float difference1 = 0;

						if (player[i]->GetWaypoint() != track->getWaypointsBB().size() - 1)
						{
							difference = Vector3::Distance(player[i]->GetPos(), track->getWaypointsBB()[player[i]->GetWaypoint() + 1].Center);
							difference1 = Vector3::Distance(player[j]->GetPos(), track->getWaypointsBB()[player[j]->GetWaypoint() + 1].Center);
						}
						else if (player[i]->GetWaypoint() == track->getWaypointsBB().size() - 1)
						{
							difference = Vector3::Distance(player[i]->GetPos(), track->getWaypointsBB()[0].Center);
							difference1 = Vector3::Distance(player[j]->GetPos(), track->getWaypointsBB()[0].Center);
						}

						if (difference > difference1)
						{
							player[i]->SetRanking(player[i]->GetRanking() + 1);
						}
					}
				}
			}
		}
	}
}

void GameScene::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  m_commandList)
{



	// temp solution for having a cinematic cam and countdown for the beta
	ID3D12DescriptorHeap* heaps[] = { Locator::getRD()->m_resourceDescriptors->Heap() };
	switch (state)
	{
		case OPENING:
			//Configure viewport
			m_commandList->RSSetViewports(1, &Locator::getWD()->sprite_viewport);
			m_commandList->RSSetScissorRects(1, &Locator::getWD()->sprite_rect);
			Locator::getRD()->m_cam = cine_cam;

			//Render 3D objects
			for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
			{
				if ((*it)->isVisible()) {
					if (dynamic_cast<Track*>(*it)) //debugging only
					{
						if (GameDebugToggles::render_level) {
							(*it)->Render();
						}
					}
					else if (dynamic_cast<DebugMarker*>(*it)) { //debugging only
						if (GameDebugToggles::show_debug_meshes) {
							(*it)->Render();
						}
					}
					else
					{
						(*it)->Render();
					}
				}
			}

			//Render items
			for (GameObject3D* obj : m_itemModels)
			{
				if (obj->isVisible()) {
					obj->Render();
				}
			}

			break;

		case COUNTDOWN:
			for (int i = 0; i < game_config["player_count"]; ++i)
			{
				//Configure viewports
				m_commandList->RSSetViewports(1, &Locator::getWD()->m_viewport[i]);
				m_commandList->RSSetScissorRects(1, &Locator::getWD()->m_scissorRect[i]);
				Locator::getRD()->m_cam = m_cam[i];

				//Render 3D objects
				for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
				{
					if ((*it)->isVisible()) {
						if (dynamic_cast<Track*>(*it)) //debugging only
						{
							if (GameDebugToggles::render_level) {
								(*it)->Render();
							}
						}
						else if (dynamic_cast<DebugMarker*>(*it)) { //debugging only
							if (GameDebugToggles::show_debug_meshes) {
								(*it)->Render();
							}
						}
						else
						{
							(*it)->Render();
						}
					}
				}

				//Render items
				for (GameObject3D* obj : m_itemModels)
				{
					if (obj->isVisible()) {
						obj->Render();
					}
				}
			}

			//Render countdown in screen centre
			m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
			m_commandList->RSSetViewports(1, &Locator::getWD()->sprite_viewport);
			m_commandList->RSSetScissorRects(1, &Locator::getWD()->sprite_rect);
			Locator::getRD()->m_spriteBatch->SetViewport(Locator::getWD()->sprite_viewport);
			Locator::getRD()->m_spriteBatch->Begin(m_commandList.Get());
			countdown_text->Render();
			Locator::getRD()->m_spriteBatch->End();
			break;

		case PLAY:
			for (int i = 0; i < game_config["player_count"]; ++i)
			{
				//Setup viewport
				m_commandList->RSSetViewports(1, &Locator::getWD()->m_viewport[i]);
				m_commandList->RSSetScissorRects(1, &Locator::getWD()->m_scissorRect[i]);
				Locator::getRD()->m_cam = m_cam[i];

				//Render 3D objects
				for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
				{
					if ((*it)->isVisible()) {
						if (dynamic_cast<Track*>(*it)) //debugging only
						{
							if (GameDebugToggles::render_level) {
								(*it)->Render();
							}
						}
						else if (dynamic_cast<DebugMarker*>(*it)) { //debugging only
							if (GameDebugToggles::show_debug_meshes) {
								(*it)->Render();
							}
						}
						else
						{
							(*it)->Render();
						}
					}
				}

				//Render items
				for (GameObject3D* obj : m_itemModels)
				{
					if (obj->isVisible()) {
						obj->Render();
					}
				}
			}

			//Render sprites
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
			break;

		case END:
			break;
	}
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
	*&Locator::getWD()->sprite_viewport = { 0.0f, 0.0f, static_cast<float>(Locator::getWD()->m_outputWidth), static_cast<float>(Locator::getWD()->m_outputHeight), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	*&Locator::getWD()->sprite_rect = { 0,0,(int)(Locator::getWD()->m_outputWidth),(int)(Locator::getWD()->m_outputHeight) };
	//test text
	//Text2D *test2 = new Text2D(m_localiser.getString("debug_text"));
	//m_2DObjects.push_back(test2);
	for (int i = 0; i < game_config["player_count"]; i++)
	{
		player[i]->GetItemImg()->SetPos(Vector2(Locator::getWD()->m_viewport[i].TopLeftX, Locator::getWD()->m_viewport[i].TopLeftY));
		//test->CentreOrigin();
		m_2DObjects.push_back(player[i]->GetItemImg());

		//player[i] = new Text2D(m_localiser.getString(std::to_string(player[i]->getCurrentWaypoint())), _RD);
		float text_pos_x = Locator::getWD()->m_viewport[i].TopLeftX + Locator::getWD()->m_viewport[i].Width - player[i]->GetRankingText()->GetSize().x;
		float text_pos_y = Locator::getWD()->m_viewport[i].TopLeftY + Locator::getWD()->m_viewport[i].Height - player[i]->GetRankingText()->GetSize().y;
		player[i]->GetRankingText()->SetPos(Vector2(text_pos_x, text_pos_y));
		m_2DObjects.push_back(player[i]->GetRankingText());

		float text_lap_x = Locator::getWD()->m_viewport[i].TopLeftX + player[i]->GetLapText()->GetSize().x * 0.5f;
		float text_lap_y = Locator::getWD()->m_viewport[i].TopLeftY + Locator::getWD()->m_viewport[i].Height - player[i]->GetLapText()->GetSize().y;
		player[i]->GetLapText()->SetPos(Vector2(text_lap_x, text_lap_y));
		m_2DObjects.push_back(player[i]->GetLapText());
	}


	countdown_text = new Text2D(std::to_string((int)std::ceil(timeout)));
	//countdown_text->CentreOrigin();
	countdown_text->SetPos({Locator::getWD()->sprite_viewport.TopLeftX + Locator::getWD()->sprite_viewport.Width / 2 - countdown_text->GetSize().x / 2 , Locator::getWD()->sprite_viewport.TopLeftY + Locator::getWD()->sprite_viewport.Height / 2 - countdown_text->GetSize().y / 2 });

	//camera_pos = new Text2D(std::to_string((int)m_cam[0]->GetPos().x) + "," + std::to_string((int)m_cam[0]->GetPos().y) + "," + std::to_string((int)m_cam[0]->GetPos().z) + "\n" + 
	//	std::to_string((int)m_cam[0]->GetOri().Translation().x) + "," + std::to_string((int)m_cam[0]->GetOri().Translation().y) + "," + std::to_string((int)m_cam[0]->GetOri().Translation().z));
	//camera_pos->SetPos({ 20,100 });
	//m_2DObjects.push_back(camera_pos);
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
	track->setWaypointBB();
	m_3DObjects.push_back(track);

	//Add all track item boxes to 3D object update array
	for (ItemBox* this_item_box : track->GetItemBoxes()) {
		m_3DObjects.push_back(this_item_box);
	}

	//Add all debug markers
	for (DebugMarker* this_debug_marker : track->GetDebugMarkers()) {
		m_3DObjects.push_back(this_debug_marker);
	}

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


	cine_cam = new Camera(Locator::getWD()->m_outputWidth, Locator::getWD()->m_outputHeight, 1.0f, 2000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	cine_cam->SetBehav(Camera::BEHAVIOUR::CINEMATIC);
	m_3DObjects.push_back(cine_cam);

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
