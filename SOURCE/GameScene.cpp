#include "pch.h"
#include "GameScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "GameDebugToggles.h"
#include "ServiceLocator.h"
#include "AudioManager.h"
#include "DebugMarker.h"
#include "Explosion.h"
#include <iostream>
#include <experimental/filesystem>
#include <memory>
#include <thread>

extern void ExitGame();

/* Create! */
GameScene::GameScene(std::string _track_name)
{
	//Get a ref to the scene manager for swapping scenes
	m_scene_manager = Locator::getSM();
	
	//Work out what track to load
	track_name = _track_name;
}

/* Destroy! */
GameScene::~GameScene()
{
	m_2DObjects.clear();
	m_3DObjects.clear();
	delete cine_cam;
	cine_cam = nullptr;
	for (int i = 0; i < 4; i++)
	{
		delete m_cam[i];
		m_cam[i] = nullptr;
	}
}

/* Load inexpensive things and create the objects for expensive things we will populate when required */
bool GameScene::Load()
{
	//Read in game config
	std::ifstream i(m_filepath.generateFilepath("GAME_CORE", m_filepath.CONFIG));
	game_config << i;

	//Read in track config
	//std::ifstream x(m_filepath.generateFilepath("MAP_CONFIG", m_filepath.CONFIG));
	//track_config << x;

	create3DObjects();
	create2DObjects();
	pushBackObjects();

	m_aiScheduler = std::make_unique<AIScheduler>(track);
	Locator::setupAIScheduler(m_aiScheduler.get());

	return true;
}

/* Populate the expensive things! */
void GameScene::ExpensiveLoad() {
	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Load();
		if (dynamic_cast<Player*>(*it)) {
			dynamic_cast<Player*>(*it)->GetAnimationMesh()->Load();
		}
		if (dynamic_cast<Track*>(*it)) {
			dynamic_cast<Track*>(*it)->LoadCollision();
		}
	}
}

/* Unpopulate the expensive things. */
void GameScene::ExpensiveUnload() {
	for (vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Reset();
		if (dynamic_cast<Track*>(*it)) {
			dynamic_cast<Track*>(*it)->UnloadCollision();
		}
	}
}

/* Create all 2D objects for the scene */
void GameScene::create2DObjects()
{
	//These widths and heights might be wrong, I dunno :)
	*&Locator::getRD()->m_screenViewport = { 0.0f, 0.0f, static_cast<float>(Locator::getRD()->m_window_width), static_cast<float>(Locator::getRD()->m_window_height), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	*&Locator::getRD()->m_scissorRect = { 0,0,(int)(Locator::getRD()->m_window_width),(int)(Locator::getRD()->m_window_height) };

	/*
	for (int i = 0; i < game_config["player_count"]; i++)
	{
		//player[i]->GetItemImg()->SetPos(Vector2(Locator::getWD()->m_viewport[i].TopLeftX, Locator::getWD()->m_viewport[i].TopLeftY));
		player[i]->SetItemPos(Vector2(Locator::getWD()->m_viewport[i].TopLeftX, Locator::getWD()->m_viewport[i].TopLeftY)); //PART OF THE GROSS MEMORY LEAK
		//test->CentreOrigin();
		m_2DObjects.push_back(player[i]->GetItemImg());

		//player[i] = new Text2D(m_localiser.getString(std::to_string(player[i]->getCurrentWaypoint())), _RD);
		float text_pos_x = Locator::getWD()->m_viewport[i].TopLeftX + Locator::getWD()->m_viewport[i].Width - player[i]->GetRankingText()->GetSize().x * 2.f;
		float text_pos_y = Locator::getWD()->m_viewport[i].TopLeftY + Locator::getWD()->m_viewport[i].Height - player[i]->GetRankingText()->GetSize().y;
		player[i]->GetRankingText()->SetPos(Vector2(text_pos_x, text_pos_y));
		m_2DObjects.push_back(player[i]->GetRankingText());

		float text_lap_x = Locator::getWD()->m_viewport[i].TopLeftX + player[i]->GetLapText()->GetSize().x * 0.25f;
		float text_lap_y = Locator::getWD()->m_viewport[i].TopLeftY + Locator::getWD()->m_viewport[i].Height - player[i]->GetLapText()->GetSize().y;
		player[i]->GetLapText()->SetPos(Vector2(text_lap_x, text_lap_y));
		m_2DObjects.push_back(player[i]->GetLapText());
	}*/
	for (int i = 0; i < game_config["player_count"]; i++)
	{
		//player[i]->GetItemImg()->SetPos(Vector2(Locator::getWD()->m_viewport[i].TopLeftX, Locator::getWD()->m_viewport[i].TopLeftY));
		player[i]->SetItemPos(Vector2(Locator::getRD()->m_screenViewport.TopLeftX, Locator::getRD()->m_screenViewport.TopLeftY)); //PART OF THE GROSS MEMORY LEAK
		//test->CentreOrigin();
		m_2DObjects.push_back(player[i]->GetItemImg());

		//player[i] = new Text2D(m_localiser.getString(std::to_string(player[i]->getCurrentWaypoint())), _RD);
		float text_pos_x = Locator::getRD()->m_screenViewport.TopLeftX + Locator::getRD()->m_screenViewport.Width - player[i]->GetRankingText()->GetSize().x * 2.f;
		float text_pos_y = Locator::getRD()->m_screenViewport.TopLeftY + Locator::getRD()->m_screenViewport.Height - player[i]->GetRankingText()->GetSize().y;
		player[i]->GetRankingText()->SetPos(Vector2(text_pos_x, text_pos_y));
		m_2DObjects.push_back(player[i]->GetRankingText());

		float text_lap_x = Locator::getRD()->m_screenViewport.TopLeftX + player[i]->GetLapText()->GetSize().x * 0.25f;
		float text_lap_y = Locator::getRD()->m_screenViewport.TopLeftY + Locator::getRD()->m_screenViewport.Height - player[i]->GetLapText()->GetSize().y;
		player[i]->GetLapText()->SetPos(Vector2(text_lap_x, text_lap_y));
		m_2DObjects.push_back(player[i]->GetLapText());
	}


	// player countdown text
	/*

	BUTCHERED AS WE ONLY HAVE ONE VIEWPORT CURRENTLY!!!

	for (int i = 0; i < game_config["player_count"]; i++)
	{
		player[i]->GetCountdown()->SetPos({ Locator::getWD()->m_viewport[i].TopLeftX + Locator::getWD()->m_viewport[i].Width / 2 - player[i]->GetCountdown()->GetSize().x / 2 , Locator::getWD()->m_viewport[i].TopLeftY + Locator::getWD()->m_viewport[i].Height / 2 - player[i]->GetCountdown()->GetSize().y / 2 });
		player[i]->GetFinishOrder()->SetPos({ Locator::getWD()->m_viewport[i].TopLeftX + Locator::getWD()->m_viewport[i].Width / 2 - player[i]->GetFinishOrder()->GetSize().x / 2 , Locator::getWD()->m_viewport[i].TopLeftY + Locator::getWD()->m_viewport[i].Height / 2 - player[i]->GetFinishOrder()->GetSize().y / 2 });
	}
	*/
	for (int i = 0; i < game_config["player_count"]; i++) {
		player[i]->GetCountdown()->SetPos({ Locator::getRD()->m_screenViewport.TopLeftX + Locator::getRD()->m_screenViewport.Width / 2 - player[i]->GetCountdown()->GetSize().x / 2 , Locator::getRD()->m_screenViewport.TopLeftY + Locator::getRD()->m_screenViewport.Height / 2 - player[i]->GetCountdown()->GetSize().y / 2 });
		player[i]->GetFinishOrder()->SetPos({ Locator::getRD()->m_screenViewport.TopLeftX + Locator::getRD()->m_screenViewport.Width / 2 - player[i]->GetFinishOrder()->GetSize().x / 2 , Locator::getRD()->m_screenViewport.TopLeftY + Locator::getRD()->m_screenViewport.Height / 2 - player[i]->GetFinishOrder()->GetSize().y / 2 });
	}
}

/* Create all 3D objects in the scene. */
void GameScene::create3DObjects()
{
	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

	//Load in a track
	track = new Track(track_name);
	track->setWaypointBB();
	m_3DObjects.push_back(track);

	//Add all track item boxes to 3D object update array
	/*
	for (ItemBox* this_item_box : track->GetItemBoxes()) {
		m_3DObjects.push_back(this_item_box);
	}*/

	//Add all debug markers
	/*
	for (DebugMarker* this_debug_marker : track->GetDebugMarkers()) {
		m_3DObjects.push_back(this_debug_marker);
	}*/

	Vector3 suitable_spawn = track->getSuitableSpawnSpot();
	for (int i = 0; i < game_config["player_count"]; i++) {

		//Create a player and position on track
		using std::placeholders::_1;
		player[i] = new Player("Knuckles_Kart", i, std::bind(&GameScene::CreateItem, this, _1));
		player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		player[i]->setMass(10);
		m_3DObjects.push_back(player[i]);

		//Create a camera to follow the player
		m_cam[i] = new Camera(Locator::getRD()->m_window_width, Locator::getRD()->m_window_height, 1.0f, 2000.0f, player[i], Vector3(0.0f, 3.0f, 10.0f));
		m_cam[i]->SetBehav(Camera::BEHAVIOUR::RACE_START);
		//m_3DObjects.push_back(m_cam[i]);

		//Create a viewport
		float width_mod = 0.5f;
		float height_mod = 0.5f;
		/*
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
		*/
		*&Locator::getRD()->m_screenViewport = { 0.0f, 0.0f, static_cast<float>(Locator::getRD()->m_window_width), static_cast<float>(Locator::getRD()->m_window_height), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
		*&Locator::getRD()->m_scissorRect = { 0,0,(int)(Locator::getRD()->m_window_width),(int)(Locator::getRD()->m_window_height) };
	}

	/*
	for (SDKMeshGO3D*& cup : debug_cups)
	{
		cup = new SDKMeshGO3D("Cup");
		m_3DObjects.push_back(cup);
	}*/


	cine_cam = new Camera(Locator::getRD()->m_window_width, Locator::getRD()->m_window_height, 1.0f, 2000.0f, nullptr, Vector3(0.0f, 3.0f, 10.0f));
	cine_cam->SetBehav(Camera::BEHAVIOUR::CINEMATIC);
	//cine_cam->SetCinematicRot(track->getCamerasRot());
	//cine_cam->SetCinematicPos(track->getCamerasPos());
	//m_3DObjects.push_back(cine_cam);

	//Global illumination
	/*
	m_light = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	m_3DObjects.push_back(m_light);
	Locator::getRD()->m_light = m_light;*/
}

/* Push objects back to their associated arrays */
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

/* Update the scene */
void GameScene::Update()
{
	m_aiScheduler->Update();
	//camera_pos->SetText(std::to_string((int)cine_cam->GetPos().x) + "," + std::to_string((int)cine_cam->GetPos().y) + "," + std::to_string((int)cine_cam->GetPos().z));


	if (finished == 4)
	{
		//m_scene_manager->setCurrentScene(Scenes::MENUSCENE);
		//Locator::getAudio()->GetSound(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_GAME)->Stop();
		//Locator::getAudio()->GetSound(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_GAME)->Stop();
		//finished = 0;
	}

	switch (state)
	{
	case START:
		Locator::getAudio()->GetSound(SOUND_TYPE::MISC, (int)SOUNDS_MISC::INTRO_MUSIC)->SetVolume(1.f);
		Locator::getAudio()->Play(SOUND_TYPE::MISC, (int)SOUNDS_MISC::INTRO_MUSIC);
		state = OPENING;
		break;
	case OPENING:
		if (timeout >= 0)
		{
			timeout -= Locator::getGSD()->m_dt;
			cine_cam->Tick();
			if (timeout <= Locator::getGSD()->m_dt + 0.1) {
				for (int i = 0; i < game_config["player_count"]; ++i) {
					m_cam[i]->Tick();
				}
			}
		}
		else
		{
			for (int i = 0; i < game_config["player_count"]; ++i) {
				m_cam[i]->Tick();
			}
			state = CAM_OPEN;
			timeout = 2.99999f;
			Locator::getAudio()->Play(SOUND_TYPE::MISC, (int)SOUNDS_MISC::PRE_COUNTDOWN);
		}
		break;
	case CAM_OPEN:
		for (int i = 0; i < game_config["player_count"]; ++i) {
			m_cam[i]->Tick();
		}
		cine_cam->Tick();

		if (m_cam[3]->GetBehav() == Camera::BEHAVIOUR::FOLLOW)
		{
			Locator::getAudio()->GetSound(SOUND_TYPE::MISC, (int)SOUNDS_MISC::COUNTDOWN)->SetVolume(0.7f);
			Locator::getAudio()->Play(SOUND_TYPE::MISC, (int)SOUNDS_MISC::COUNTDOWN);
			state = COUNTDOWN;
		}
		break;
	case COUNTDOWN:
		if (timeout > 0)
		{
			timeout -= Locator::getGSD()->m_dt;
			for (int i = 0; i < game_config["player_count"]; ++i) {
				std::string countdown_time = std::to_string((int)std::ceil(timeout));
				if (countdown_time == "0") {
					countdown_time = "GO!";
				}
				player[i]->GetCountdown()->SetText(countdown_time);
				m_cam[i]->Tick();
			}
		}
		else
		{
			state = PLAY;
			timeout = 3.5f;
			Locator::getAudio()->GetSound(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_START)->SetVolume(0.7f);
			Locator::getAudio()->Play(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_START);
			for (int i = 0; i < game_config["player_count"]; ++i)
			{
				player[i]->setGamePad(true);
			}
		}
		break;
	case PLAY:
		for (int i = 0; i < game_config["player_count"]; ++i) {
			m_cam[i]->Tick();
		}

		timeout -= Locator::getGSD()->m_dt;

		if (timeout <= 0 && track_music_start)
		{
			Locator::getAudio()->Play(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_GAME);
			track_music_start = false;
		}

		if (timeout <= 0 && final_lap_start)
		{
			Locator::getAudio()->Play(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_FL_GAME);
			final_lap_start = false;
		}

		break;
	}

	for (int i = 0; i < game_config["player_count"]; ++i) {
		player[i]->ShouldStickToTrack(*track);
		player[i]->ResolveWallCollisions(*track);
		Locator::getID()->m_gamePadState[i] = Locator::getID()->m_gamepad->GetState(i); //set game controllers state[s]

	}

	if (m_keybinds.keyPressed("Quit"))
	{
		Locator::getAudio()->GetSound(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_GAME)->Stop();
		Locator::getAudio()->GetSound(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_FL_GAME)->Stop();
		m_scene_manager->setCurrentScene(Scenes::MENUSCENE);
	}
	if (m_keybinds.keyPressed("Orbit"))
	{
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::INDEPENDENT);
	}
	if (m_keybinds.keyPressed("Lerp"))
	{
		m_cam[0]->SetBehav(Camera::BEHAVIOUR::FOLLOW);
	}
	if (m_keybinds.keyPressed("Matt"))
	{
		if (m_cam[0]->GetBehav() == Camera::BEHAVIOUR::DEBUG_CAM) {
			m_cam[0]->SetBehav(Camera::BEHAVIOUR::FOLLOW);
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
		PhysModel* model = dynamic_cast<PhysModel*>(m_3DObjects[delIndex]);
		if (model)
		{
			for (int j = 0; j < m_physModels.size(); ++j)
			{
				if (model == m_physModels[j])
				{
					m_physModels.erase(m_physModels.begin() + j);
					break;
				}
			}
		}

		//Locator::getGarbageCollector()->DeletePointer(m_3DObjects[delIndex]);
		m_3DObjects.erase(m_3DObjects.begin() + delIndex);
	}

	/*end = m_physModels.size();
	delIndex = -1;
	for (int i = 0; i < end; i++)
	{
		if (m_physModels[i]->ShouldDestroy())
		{
			delIndex = i;
		}
	}
	if (delIndex != -1)
	{
		Locator::getGarbageCollector()->DeletePointer(m_physModels[delIndex]);
		m_physModels.erase(m_physModels.begin() + delIndex);
	}*/

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
		timeout = 2.999999f;
		state = COUNTDOWN;
	}

	for (int i = 0; i < 8; i++)
	{
		switch (i)
		{
			//case 0:
			//	debug_cups[i]->SetPos(player[0]->data.m_globalFrontTopLeft);
			//	break;
			//case 1:
			//	debug_cups[i]->SetPos(player[0]->data.m_globalFrontTopRight);
			//	break;
			//case 2:
			//	debug_cups[i]->SetPos(player[0]->data.m_globalFrontBottomLeft);
			//	break;
			//case 3:
			//	debug_cups[i]->SetPos(player[0]->data.m_globalFrontBottomRight);
			//	break;
			//case 4:
			//	debug_cups[i]->SetPos(player[0]->data.m_globalBackTopLeft);
			//	break;
			//case 5:
			//	debug_cups[i]->SetPos(player[0]->data.m_globalBackTopRight);
			//	break;
			//case 6:
			//	debug_cups[i]->SetPos(player[0]->data.m_globalBackBottomLeft);
			//	break;
			//case 7:
			//	debug_cups[i]->SetPos(player[0]->data.m_globalBackBottomRight);
			//	break;
		}
	}

	CollisionManager::CollisionDetectionAndResponse(m_physModels, m_itemModels);

	UpdateItems();
}

/* Update the items in the scene specifically */
void GameScene::UpdateItems()
{
	int delIndex = -1;
	int end = m_itemModels.size();
	for (int i = 0; i < end; i++)
	{
		if (m_itemModels[i]->GetMesh())
		{
			m_itemModels[i]->GetMesh()->ShouldStickToTrack(*track);
			m_itemModels[i]->GetMesh()->ResolveWallCollisions(*track);
		}
		/*for (int j = 0; j < game_config["player_count"]; ++j) {
			if (m_itemModels[i]->GetMesh() && player[j]->getCollider().Intersects(m_itemModels[i]->GetMesh()->getCollider()))
			{
				m_itemModels[i]->HitByPlayer(player[j]);
			}
		}*/
		m_itemModels[i]->Tick();
		if (m_itemModels[i]->ShouldDestroy())
		{
			delIndex = i;
		}

		/*if (m_itemModels[i]->GetMesh())
		{
			int end2 = m_itemModels.size();
			for (int j = 0; j < end2; j++)
			{
				if (m_itemModels[i] != m_itemModels[j] && m_itemModels[j]->GetMesh() && m_itemModels[j]->GetMesh()->getCollider().Intersects(m_itemModels[i]->GetMesh()->getCollider()))
				{
					m_itemModels[i]->FlagForDestoy();
					m_itemModels[j]->FlagForDestoy();
				}
			}
		}*/
	}
	if (delIndex != -1)
	{
		//Locator::getGarbageCollector()->DeletePointer(m_itemModels[delIndex]);
		m_itemModels.erase(m_itemModels.begin() + delIndex);
	}
}

/* Render the scene */
void GameScene::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	/*
		This has been kinda butchered for now as we only have one viewport, not 4.
	*/

	// temp solution for having a cinematic cam and countdown for the beta
	ID3D12DescriptorHeap* heaps[] = { Locator::getRD()->m_resourceDescriptors->Heap() };
	switch (state)
	{
	case OPENING:
		//Configure viewport
		m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
		m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);
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
		for (Item* obj : m_itemModels)
		{
			if (obj->GetRenderMesh())
			{
				obj->GetRenderMesh()->Render();
			}
		}
		m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

		//Render countdown in screen centre
		/*
		m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
		m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);
		Locator::getRD()->m_spriteBatch->SetViewport(Locator::getRD()->m_screenViewport);
		Locator::getRD()->m_spriteBatch->Begin(m_commandList.Get());
		Locator::getRD()->m_spriteBatch->End();*/

		break;
	case CAM_OPEN:
	case COUNTDOWN:
		//for (int i = 0; i < game_config["player_count"]; ++i)
		//{
			//Configure viewports
			//m_commandList->RSSetViewports(1, &Locator::getWD()->m_viewport[i]);
			//m_commandList->RSSetScissorRects(1, &Locator::getWD()->m_scissorRect[i]);
			//Locator::getRD()->m_cam = m_cam[i];

		m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
		m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);
		Locator::getRD()->m_cam = m_cam[0];

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
		for (Item* obj : m_itemModels)
		{
			if (obj->GetMesh())
			{
				obj->GetRenderMesh()->Render();
			}
		}
		//}
		m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);

		if (state == COUNTDOWN)
		{
			//Render countdown in screen centre
			/*
			m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
			m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);
			Locator::getRD()->m_spriteBatch->SetViewport(Locator::getRD()->m_screenViewport);
			Locator::getRD()->m_spriteBatch->Begin(m_commandList.Get());
			for (int i = 0; i < game_config["player_count"]; i++)
			{
				player[i]->GetCountdown()->Render();
			}
			Locator::getRD()->m_spriteBatch->End();*/
		}
		break;
	case PLAY:
		//for (int i = 0; i < game_config["player_count"]; ++i)
		//{
			//Setup viewport
			//m_commandList->RSSetViewports(1, &Locator::getWD()->m_viewport[i]);
			//m_commandList->RSSetScissorRects(1, &Locator::getWD()->m_scissorRect[i]);
			//Locator::getRD()->m_cam = m_cam[i];

		m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
		m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);
		Locator::getRD()->m_cam = m_cam[0];

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
		for (Item* obj : m_itemModels)
		{
			if (obj->GetMesh())
			{
				obj->GetRenderMesh()->Render();
			}
		}
		//}

		//Render sprites
		/*
		m_commandList->SetDescriptorHeaps(_countof(heaps), heaps);
		m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
		m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);
		Locator::getRD()->m_spriteBatch->SetViewport(Locator::getRD()->m_screenViewport);
		Locator::getRD()->m_spriteBatch->Begin(m_commandList.Get());
		//draw 2d objects
		//for (vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
		//{
		//	(*it)->Render();
		//}
		for (int i = 0; i < game_config["player_count"]; i++)
		{
			if (player[i]->GetFinished())
			{
				player[i]->GetFinishOrder()->Render();
			}
			else if (!player[i]->GetFinished())
			{
				player[i]->GetRankingText()->Render();
				player[i]->GetLapText()->Render();
				player[i]->GetItemImg()->Render();
			}
		}
		Locator::getRD()->m_spriteBatch->End();*/
		break;
	}
}

/* Set the player's current waypoint */
void GameScene::SetPlayersWaypoint()
{
	for (int i = 0; i < game_config["player_count"]; i++) {

		if (player[i]->GetWaypoint() < track->getWaypointsBB().size() - 1)
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
				if (player[i]->GetLap() == 3)
				{
					player[i]->SetFinished(true);
					player[i]->GetFinishOrder()->SetText(std::to_string(player[i]->GetRanking()) + player[i]->GetOrderIndicator()[player[i]->GetRanking() - 1]);
					m_cam[i]->SetBehav(Camera::BEHAVIOUR::ORBIT);
					player[i]->setGamePad(false);
					finished++;
				}
				else if (player[i]->GetLap() == 2 && !final_lap)
				{
					Locator::getAudio()->GetSound(SOUND_TYPE::GAME, (int)SOUNDS_GAME::MKS_GAME)->Stop();
					Locator::getAudio()->Play(SOUND_TYPE::MISC, (int)SOUNDS_MISC::FINAL_LAP_IND);
					final_lap = true;
					final_lap_start = true;
					timeout = 3.8f;
				}
				player[i]->SetLap(player[i]->GetLap() + 1);
			}
		}
	}
}

/* Set the player's current position in the race */
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

/* Create an item in the scene */
Item* GameScene::CreateItem(ItemType type)
{
	switch (type)
	{
	case BANANA:
	{
		Banana* banana = new Banana();
		m_itemModels.push_back(banana);
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(banana->GetMesh())->getDebugCollider());
		return banana;
	}
	case GREEN_SHELL:
	{
		GreenShell* greenShell = new GreenShell();
		m_itemModels.push_back(greenShell);
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(greenShell->GetMesh())->getDebugCollider());

		return greenShell;
	}
	case MUSHROOM:
	{
		Mushroom * mushroom = new Mushroom();
		m_itemModels.push_back(mushroom);
		return mushroom;
	}
	case BOMB:
	{
		Bomb* bomb = new Bomb(std::bind(&GameScene::CreateExplosion, this));
		m_itemModels.push_back(bomb);
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(bomb->GetMesh())->getDebugCollider());

		return bomb;
	}
	case FAKE_BOX:
	{
		FakeItemBox* box = new FakeItemBox();
		m_itemModels.push_back(box);
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(box->GetMesh())->getDebugCollider());
		return box;
	}
	default:
		return nullptr;
	}
}

/* Create an explosion! */
Explosion * GameScene::CreateExplosion()
{
	Explosion* explosion = new Explosion();
	m_3DObjects.push_back(explosion);
	m_physModels.push_back(explosion);
	m_3DObjects.push_back(dynamic_cast<PhysModel*>(explosion)->getDebugCollider());
	return explosion;
}

/* Other crap which I don't think we use anymore */
void GameScene::DeleteItem(Item * item)
{
	std::thread thread(&GameScene::DeleteThread, this, item);
	thread.detach();
}
void GameScene::DeleteThread(Item * item)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	delete item;
}