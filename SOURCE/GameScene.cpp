#include "pch.h"
#include "GameScene.h"
#include "GameStateData.h"
#include "RenderData.h"
#include "SceneManager.h"
#include "GameDebugToggles.h"
#include "ServiceLocator.h"
#include "AudioManager.h"
#include "DebugMarker.h"
#include "CameraData.h"
#include "RaceManager.h"
#include "Explosion.h"
#include "GameObjectShared.h"
#include "AIScheduler.h"
#include <iostream>
#include <experimental/filesystem>
#include <memory>
#include <thread>

extern void ExitGame();

/* Create! */
GameScene::GameScene(MapInfo* _track) {
	//Get a ref to the scene manager for swapping scenes
	m_scene_manager = Locator::getSM();
	
	//Our map's info
	map_info = _track; //Use this to load the audio in ExpensiveLoad!
}

/* Destroy! */
GameScene::~GameScene()
{
	m_2DObjects.clear();
	m_3DObjects.clear();
	delete cine_cam;
	cine_cam = nullptr;
	for (int i = 0; i < Locator::getRM()->player_amount; i++)
	{
		delete m_cam[i];
		m_cam[i] = nullptr;
	}
	delete m_itemPools;
	m_itemPools = nullptr;
}

/* Load inexpensive things and create the objects for expensive things we will populate when required */
bool GameScene::Load()
{
	//Read in game config
	std::ifstream i(m_filepath.generateFilepath("GAME_CORE", m_filepath.CONFIG));
	game_config << i;

	create3DObjects();
	create2DObjects();
	pushBackObjects();
	CollisionManager::InitConfig();

	return true;
}

/* Populate the expensive things! */
void GameScene::ExpensiveLoad() {
	//Set cubemaps
	Locator::getRD()->current_cubemap_radiance = map_info->cubemap_radiance;
	Locator::getRD()->current_cubemap_irradiance = map_info->cubemap_irradiance;
	Locator::getRD()->current_cubemap_skybox = map_info->cubemap_skybox;

	if (Locator::getRM()->attract_state)
	{
		m_maxPlayers = 1;
	}
	else
	{
		m_maxPlayers = 1;
	}

	//Update characters
	for (int i = 0; i < Locator::getRM()->player_amount; i++)
	{
		if(!Locator::getRM()->attract_state)
			player[i]->SetPlayerID(i);

		player[i]->SetLap(1);
		player[i]->Reload(
			Locator::getGOS()->character_instances.at(Locator::getGSD()->character_selected[i]),
			Locator::getGOS()->vehicle_instances.at(Locator::getGSD()->vehicle_selected[i])
		);
		if (track->getSpawnRotations().size() > 0) {
			player[i]->setVelocity(Matrix::CreateWorld(player[i]->GetPos(), track->getSpawnRotations()[0] * -2, Vector3::Up).Forward());
			player[i]->UpdateWorld();
			player[i]->SetPos(Vector3(track->getSuitableSpawnSpot().x, track->getSuitableSpawnSpot().y, track->getSuitableSpawnSpot().z) - player[i]->getVelocity() * i * 3);
		}
	}
	for (int i = Locator::getRM()->player_amount; i < m_maxPlayers; i++)
	{
		player[i]->SetPlayerID(-1);
		player[i]->SetLap(1);
		player[i]->Reload(
			Locator::getGOS()->character_instances.at(Locator::getGSD()->character_selected[0]),
			Locator::getGOS()->vehicle_instances.at(Locator::getGSD()->vehicle_selected[0])
		);
		if (track->getSpawnRotations().size() > 0) {
			player[i]->setVelocity(Matrix::CreateWorld(player[i]->GetPos(), track->getSpawnRotations()[0] * -2, Vector3::Up).Forward());
			player[i]->UpdateWorld();
			player[i]->SetPos(Vector3(track->getSuitableSpawnSpot().x, track->getSuitableSpawnSpot().y, track->getSuitableSpawnSpot().z) - player[i]->getVelocity() * i * 3);
		}
	}

	//Setup audio
	Locator::getAudio()->addToSoundsList(map_info->audio_background_start, "TRACK_START");
	Locator::getAudio()->addToSoundsList(map_info->audio_background, "TRACK_LOOP");
	Locator::getAudio()->addToSoundsList(map_info->audio_final_lap_start, "FINAL_LAP_START");
	Locator::getAudio()->addToSoundsList(map_info->audio_final_lap, "FINAL_LAP_LOOP");

	//Load main UI
	for (int i = 0; i < Locator::getRM()->player_amount; i++) {
		delete m_game_ui[i];
		m_game_ui[i] = new InGameUI(Vector2(1280,720), Vector2(0,0));
		m_game_ui[i]->ExpensiveLoad();
		m_game_ui[i]->SetCurrentLap(1);
		m_game_ui[i]->SetPlayerPosition(1);
		m_game_ui[i]->SetMapName(map_info->name);
	}

	//Position countdown
	for (int i = 0; i < Locator::getRM()->player_amount; i++)
	{
		player[i]->GetCountdown()->SetPos(
			Vector2(
				Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX + Locator::getRD()->m_screenViewportSplitscreen[i].Width / 2 - player[i]->GetCountdown()->GetSize().x / 2 ,
				Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY + Locator::getRD()->m_screenViewportSplitscreen[i].Height / 2 - player[i]->GetCountdown()->GetSize().y / 2
			) / (static_cast<float>(Locator::getRD()->m_window_height) / 720), false);
		player[i]->GetFinishOrder()->SetPos(
			Vector2(
				Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX + Locator::getRD()->m_screenViewportSplitscreen[i].Width / 2 - player[i]->GetFinishOrder()->GetSize().x / 2 ,
				Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY + Locator::getRD()->m_screenViewportSplitscreen[i].Height / 2 - player[i]->GetFinishOrder()->GetSize().y / 2
			) / (static_cast<float>(Locator::getRD()->m_window_height) / 720), false);
		m_cam[i]->ResetFOV();
	}

	//Load in
	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		//Load meshes
		(*it)->Load();
		if (dynamic_cast<Player*>(*it) && dynamic_cast<Player*>(*it)->GetAnimController()) {
			dynamic_cast<Player*>(*it)->ExpensiveLoad();
		}
		//Load collision info
		if (dynamic_cast<Track*>(*it)) {
			dynamic_cast<Track*>(*it)->LoadCollision();
		}
	}

	if (!m_itemPools)
	{
		//Setup item pools
		m_itemPools = new ItemPools();
		Locator::setupItemPools(m_itemPools);
	}
	else
	{
		Locator::getItemPools()->Load();
	}
	

	//Set AI to current track
	Locator::getAIScheduler()->UpdateTrack(track);

	//Reset key presses
	m_keybinds.Reset();

	//Load skybox
	Locator::getRD()->skybox->Load();


	for (auto& camera : track->getIntroCams())
	{
		std::vector<Vector3> camera_pos;

		camera_pos.push_back(camera.start_pos);
		camera_pos.push_back(camera.end_pos);
		Locator::getCD()->points.push_back(camera_pos);
		Locator::getCD()->look_points.push_back(camera.look_at != Vector3(0, 0, 0) ? camera.look_at : Vector3(1, 1, 1));
	}
}

/* Unpopulate the expensive things. */
void GameScene::ExpensiveUnload() {
	Vector3 suitable_spawn = Vector3(0, 0, 0);

	//Unload meshes
	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Reset();
		if (dynamic_cast<Track*>(*it)) {
			dynamic_cast<Track*>(*it)->Reset();
			suitable_spawn = dynamic_cast<Track*>(*it)->getSuitableSpawnSpot();
		}
	}

	//Reset player positions & camera mode
	for (int i = 0; i < m_maxPlayers; i++) {
		player[i]->SetWaypoint(0);
		player[i]->SetLap(1);
		player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		if (i < 4)
		{
			m_cam[i]->Reset();
			m_cam[i]->SetType(CameraType::FOLLOW);
		}
	}

	cine_cam->Reset();
	cine_cam->SetType(CameraType::CINEMATIC);

	Locator::getAudio()->clearTrackSounds();
	Locator::getCD()->look_points.clear();
	Locator::getCD()->points.clear();

	//We'll probably need to reset more stuff here, like race timers, etc
	timeout = 12.f;
	finish_timer = 0;
	state = START;
	m_playerControls = false;
	track_music_start = true;
	final_lap_start = false;
	final_lap = false;
	finished = 0;
	is_paused = false;
	race_finished = false;

	//Unload main UI
	for (int i = 0; i < Locator::getRM()->player_amount; i++) {
		m_game_ui[i]->ExpensiveUnload();
	}

	//Unload skybox
	Locator::getRD()->skybox->Reset();

	if (!Locator::getRM()->attract_state)
	{
		//Unload item pools
		for (int i = 0; i < m_itemModels.size(); ++i)
		{
			if (m_itemModels[i]->GetItemMesh())
			{
				Locator::getItemPools()->AddItemMesh(m_itemModels[i]->GetItemType(), m_itemModels[i]->GetItemMesh());
				if (m_itemModels[i]->GetItemType() == BOMB)
				{
					Locator::getItemPools()->AddExplosion(dynamic_cast<Bomb*>(m_itemModels[i])->GetExplosion()->GetDisplayedMesh());
				}
			}

		}
		m_itemModels.clear();
		Locator::getItemPools()->Reset();
	}
}

/* Create all 2D objects for the scene */
void GameScene::create2DObjects()
{
	m_pause_screen = new ImageGO2D("paused");

	for (int i = 0; i < Locator::getRM()->player_amount; i++)
	{
		player[i]->SetItemPos(Vector2(Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX, Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY));
		m_2DObjects.push_back(player[i]->GetItemImg());
	}
}

/* Create all 3D objects in the scene. */
void GameScene::create3DObjects()
{
	//Load in a track
	track = new Track(map_info);
	track->setWaypointBB();
	m_3DObjects.push_back(track);

	//Add all track item boxes to 3D object update array
	for (ItemBox* this_item_box : track->GetItemBoxes()) {
		m_3DObjects.push_back(this_item_box);
	}

	//Add all debug markers
	#ifdef _DEBUG
	for (DebugMarker* this_debug_marker : track->GetDebugMarkers()) {
		m_3DObjects.push_back(this_debug_marker);
	}
	#endif

	// Spawn in the AI
	for (int i = 0; i < m_maxPlayers; i++) {

		//Create a player and position on track
		using std::placeholders::_1;
		player[i] = new Player(
			Locator::getGOS()->character_instances.at(Locator::getGSD()->character_selected[0]),
			Locator::getGOS()->vehicle_instances.at(Locator::getGSD()->vehicle_selected[0]),
			-1, std::bind(&GameScene::CreateItem, this, _1)
		);
		player[i]->SetPos(Vector3(track->getSuitableSpawnSpot().x, track->getSuitableSpawnSpot().y, track->getSuitableSpawnSpot().z - (i * 10)));
		player[i]->setMass(10);
		m_3DObjects.push_back(player[i]);

		if (i < 4)
		{
			m_cam[i] = new Camera(Locator::getRD()->m_window_width, Locator::getRD()->m_window_height, Vector3(0.0f, 3.0f, 10.0f), player[i], CameraType::FOLLOW);
			m_cam[i]->setAngle(180.0f);
		}
	}

	//Cinematic cam
	cine_cam = new Camera(Locator::getRD()->m_window_width, Locator::getRD()->m_window_height, Vector3(0.0f, 3.0f, 10.0f), nullptr, CameraType::CINEMATIC);
	cine_cam->SetType(CameraType::CINEMATIC);
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

//Gets all players in 3DObject vector
std::vector<Player*> GameScene::GetPlayers()
{
	std::vector<Player*> players;

	for (auto& obj : m_3DObjects)
	{
		Player* player = dynamic_cast<Player*>(obj);
		if (player)
		{
			players.push_back(player);
		}
	}

	return players;
}

/* Update the scene */
void GameScene::Update(DX::StepTimer const& timer)
{
	//handle pause
	if (is_paused) {
		if (m_keybinds.keyReleased("pause")) {
			is_paused = false;
		}
		return;
	}
	if (m_keybinds.keyReleased("pause")) {
		is_paused = true;
	}

	if (!Locator::getRM()->attract_state)
	{
		int players_finished = 0;
		for (size_t i = 0; i < Locator::getRM()->player_amount; ++i)
		{
			if (player[i]->GetLap() == 4)
			{
				m_game_ui[i]->SetState(InGameInterfaceState::UI_RACE_OVER);
				players_finished++;
			}
		}

		if (!race_finished)
			race_finished = players_finished == Locator::getRM()->player_amount;


		if (race_finished)
		{
			finish_timer += (float)timer.GetElapsedSeconds();

			if (finish_timer > 10)
			{
				Locator::getAudio()->StopAll();
				Locator::getRM()->attract_state = false;
				m_scene_manager->setCurrentScene(Scenes::LOADINGSCENE);
				return;
			}
		}
	}

	//Update UI
	for (int i = 0; i < Locator::getRM()->player_amount; i++) {
		if (player[i]->DidUseItem()) {
			m_game_ui[i]->HideItemSpinner();
		}
		if (m_game_ui[i] != nullptr) {
			m_game_ui[i]->Update();
		}
	}

	Locator::getAIScheduler()->Update();

	if (Locator::getRM()->attract_state)
	{
		if (m_keybinds.keyReleased("Activate")) {
			Locator::getRM()->attract_state = false;
			m_scene_manager->setCurrentScene(Scenes::LOADINGSCENE);
			return;
		}
	}

	switch (state)
	{
	case START:
		for (int i = 0; i < Locator::getRM()->player_amount; i++) {
			m_game_ui[i]->SetState(InGameInterfaceState::UI_COURSE_INTRO);
		}

		if (!Locator::getRM()->attract_state)
			Locator::getAudio()->Play("COURSE_INTRO");

		state = OPENING;
		break;
	case OPENING:
		if (timeout >= 0)
		{
			timeout -= Locator::getGSD()->m_dt;
			cine_cam->Tick();
			if (timeout <= Locator::getGSD()->m_dt + 0.1) {
				for (int i = 0; i < Locator::getRM()->player_amount; ++i) {
					m_cam[i]->Tick();
					m_game_ui[i]->SetState(InGameInterfaceState::UI_COURSE_INTRO);
				}
			}
		}
		else
		{
			for (int i = 0; i < Locator::getRM()->player_amount; ++i) {
				m_cam[i]->Tick();
				m_game_ui[i]->SetState(InGameInterfaceState::UI_COURSE_INTRO);
			}
			state = CAM_OPEN;
			timeout = 2.99999f;
			if (!Locator::getRM()->attract_state)
				Locator::getAudio()->Play("PRE_COUNTDOWN");

			if (Locator::getRM()->player_amount == 3)
			{
				cine_cam->SetType(CameraType::ORBIT);
				cine_cam->SetTarget(player[3]);
			}
		}
		#ifdef _DEBUG
		if (m_keybinds.keyReleased("Activate"))
		{
			timeout = 2.999999f;
			state = COUNTDOWN;
		}
		#endif
		break;
	case CAM_OPEN:
		for (int i = 0; i < Locator::getRM()->player_amount; ++i) {
			m_cam[i]->Tick();
			m_game_ui[i]->SetState(InGameInterfaceState::UI_COURSE_INTRO);
		}

		if (Locator::getRM()->player_amount == 3)
		{
			cine_cam->Tick();
		}

		if (m_cam[game_config["player_count"]-1]->GetType() == CameraType::FOLLOW)
		{
			if (!Locator::getRM()->attract_state)
				Locator::getAudio()->Play("COUNTDOWN");
			state = COUNTDOWN;
		}
		break;
	case COUNTDOWN:
		if (timeout > 0)
		{
			timeout -= Locator::getGSD()->m_dt;
			for (int i = 0; i < Locator::getRM()->player_amount; ++i) {
				std::string countdown_time = std::to_string((int)std::ceil(timeout));
				if (countdown_time == "0") {
					countdown_time = "GO!";
				}
				player[i]->GetCountdown()->SetText(countdown_time);
				m_cam[i]->Tick();
				m_game_ui[i]->SetState(InGameInterfaceState::UI_COUNTDOWN);
			}

			if (Locator::getRM()->player_amount == 3)
			{
				cine_cam->Tick();
			}
		}
		else
		{
			state = PLAY;
			timeout = 3.5f;
			if(!Locator::getRM()->attract_state)
				Locator::getAudio()->Play("TRACK_START");

			for (int i = 0; i < m_maxPlayers; ++i)
			{
				player[i]->setGamePad(true);
			}

		}
		break;
	case PLAY:
		for (int i = 0; i < Locator::getRM()->player_amount; ++i) {
			m_cam[i]->Tick();
			m_game_ui[i]->SetState(InGameInterfaceState::UI_RACING);
		}


		if (Locator::getRM()->player_amount == 3)
		{
			cine_cam->Tick();
		}

		timeout -= Locator::getGSD()->m_dt;

		if (!Locator::getRM()->attract_state)
		{
			if (timeout <= 0 && track_music_start)
			{
				Locator::getAudio()->Play("TRACK_LOOP");
				track_music_start = false;
			}

			if (timeout <= 0 && final_lap_start)
			{
				Locator::getAudio()->Play("FINAL_LAP_LOOP");
				final_lap_start = false;
			}
		}
		break;
	}

	for (int i = 0; i < m_maxPlayers; ++i) {
		player[i]->ShouldStickToTrack(*track);
		//player[i]->ResolveWallCollisions(*track);
	}

	if (m_keybinds.keyReleased("Quit"))
	{
		Locator::getAudio()->StopAll();
		m_scene_manager->setCurrentScene(Scenes::MENUSCENE);
	}
	if (m_keybinds.keyReleased("toggle orbit cam"))
	{
		m_cam[0]->SetType(CameraType::INDEPENDENT);
	}
	if (m_keybinds.keyReleased("toggle lerp cam"))
	{
		m_cam[0]->SetType(CameraType::FOLLOW);
	}
#ifdef _DEBUG
	if (m_keybinds.keyReleased("toggle debug cam"))
	{
		if (m_cam[0]->GetType() == CameraType::DEBUG_CAM) {
			m_cam[0]->SetType(CameraType::FOLLOW);
			return;
		}
		m_cam[0]->SetType(CameraType::DEBUG_CAM);
	}
#endif

	// sets the players waypoint
	SetPlayersWaypoint();

	// Sets the current position of the player on teh leaderboard
	SetPlayerRanking();

	for (std::vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
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
			if (dynamic_cast<Explosion*>(m_3DObjects[i]))
			{
				Locator::getItemPools()->AddExplosion(dynamic_cast<Explosion*>(m_3DObjects[i])->GetDisplayedMesh());
			}

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

		m_3DObjects.erase(m_3DObjects.begin() + delIndex);
	}

	//Toggle debug mesh renders
	if (m_keybinds.keyReleased("toggle collision debug"))
	{
		GameDebugToggles::show_debug_meshes = !GameDebugToggles::show_debug_meshes;
		DebugText::print("show_debug_meshes: " + std::to_string((int)GameDebugToggles::show_debug_meshes));
	}
	if (m_keybinds.keyReleased("toggle world render")) {
		GameDebugToggles::render_level = !GameDebugToggles::render_level;
	}

	////Handle collisions and act on types for gamescene objects
	/*for (Collision& this_collision : CollisionManager::CollisionDetection(m_physModels, m_itemModels)) {
		switch (CollisionManager::CollisionResponse(&this_collision)) {
			//If we hit an item box, perform the UI animation and give the player their item
			case PlayerCollisionTypes::HIT_ITEMBOX: {
				Player* this_player = nullptr;
				if (dynamic_cast<Player*>(this_collision.m_model1)) {
					this_player = dynamic_cast<Player*>(this_collision.m_model1);
				} else {
					this_player = dynamic_cast<Player*>(this_collision.m_model2);
				}
				if (this_player == nullptr) { DebugText::print("Failed to reference player for item box collision."); return; }
			m_game_ui[this_player->GetPlayerId()]->ShowItemSpinner(this_player);
			}
		}
	}*/
	CollisionManager::CollisionDetectionAndResponse(m_physModels, m_itemModels, m_game_ui);
	UpdateItems();
}

/* Update the items in the scene specifically */
void GameScene::UpdateItems()
{
	for (int i = 0; i < m_itemModels.size(); i++)
	{
		if (m_itemModels[i]->GetItemMesh())
		{
			m_itemModels[i]->GetMesh()->ShouldStickToTrack(*track);
			m_itemModels[i]->GetMesh()->ResolveWallCollisions(*track);
		}
		m_itemModels[i]->Tick();
		if (m_itemModels[i]->ShouldDestroy())
		{
			if (m_itemModels[i]->GetItemMesh())
			{
				Locator::getItemPools()->AddItemMesh(m_itemModels[i]->GetItemType(), m_itemModels[i]->GetItemMesh());
			}

			//Removes the AI from the red shell
			if (dynamic_cast<RedShell*>(m_itemModels[i]))
			{
				dynamic_cast<RedShell*>(m_itemModels[i])->GetAi().reset();
			}

			m_itemModels.erase(m_itemModels.begin() + i);
		}
	}
}

/* Render the 3D scene */
void GameScene::Render3D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{

	switch (state)
	{
	case OPENING:
		//Configure viewport
		m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
		m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);
		Locator::getRD()->m_cam = cine_cam;

		render3DObjects();

		break;
	case CAM_OPEN:
	case COUNTDOWN:
	case PLAY:
		for (int i = 0; i < Locator::getRM()->player_amount; ++i)
		{
			//Configure viewports
			m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRectSplitscreen[i]);
			m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewportSplitscreen[i]);
			Locator::getRD()->m_cam = m_cam[i];

			render3DObjects();
		}

		if (Locator::getRM()->player_amount == 3)
		{
			m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRectSplitscreen[3]);
			m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewportSplitscreen[3]);
			Locator::getRD()->m_cam = cine_cam;
			render3DObjects();
		}

		break;
	}
	m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewport);
	m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRect);
}

void GameScene::render3DObjects()
{
	// Render skybox
	Locator::getRD()->skybox->Render();

	//Render 3D objects
	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		if ((*it)->isVisible()) {
			if (dynamic_cast<Track*>(*it))
			{
				if (GameDebugToggles::render_level) {
					(*it)->Render();
				}
			}
#ifdef _DEBUG
			else if (dynamic_cast<DebugMarker*>(*it)) { //debugging only
				if (GameDebugToggles::show_debug_meshes) {
					(*it)->Render();
				}
			}
#endif
			else if (dynamic_cast<Explosion*>(*it))
			{
				(*it)->Render();
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
		if (obj->GetItemMesh())
		{
			obj->Render();
		}
	}
}

/* Render the 2D scene */
void GameScene::Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	if (Locator::getRM()->attract_state)
	{
		return;
	}

	if (is_paused) {
		m_pause_screen->Render();
		return;
	}


	/* TODO: this needs to be refactored in favour of InGameUI */
	switch (state)
	{
	case COUNTDOWN:
		//Render countdown in screen centre
		for (int i = 0; i < Locator::getRM()->player_amount; i++)
		{
			player[i]->GetCountdown()->Render();
		}
		break;
	case PLAY:
		for (int i = 0; i < Locator::getRM()->player_amount; i++)
		{
			if (player[i]->GetFinished())
			{
				player[i]->GetFinishOrder()->Render();
			}
			else if (!player[i]->GetFinished())
			{
				if(player[i]->GetItemInInventory() != ItemType::NONE)
					player[i]->GetItemImg()->Render();
			}
		}
		break;
	}
	/* ^^^^^^^^^^^^^^^^^ depreciate! */


	//Render UI
	for (int i = 0; i < Locator::getRM()->player_amount; i++) {
		if (m_game_ui[i] != nullptr) {
			m_game_ui[i]->Render();
		}
	}
}

/* Set the player's current waypoint */
void GameScene::SetPlayersWaypoint()
{
	for (int i = 0; i < m_maxPlayers; i++) {
		Vector3 difference = player[i]->GetPosHistoryBack() - player[i]->GetPos();
		float maxLength = difference.Length();
		float length = 0;
		if (maxLength == 0)
			return;

		difference.Normalize();

		if (player[i]->GetWaypoint() < track->getWaypointsBB().size() - 1)
		{
			if (track->getWaypointsBB()[player[i]->GetWaypoint() + 1].Intersects(player[i]->GetPos(), difference, length))
			{
				if(length <= maxLength)
					player[i]->SetWaypoint(player[i]->GetWaypoint() + 1);
			}
		}
		else
		{
			if (track->getWaypointsBB()[0].Intersects(player[i]->GetPos(), difference, length))
			{
				player[i]->SetWaypoint(0);
				if (player[i]->GetLap() == 3)
				{
					player[i]->SetFinished(true);
					player[i]->GetFinishOrder()->SetText(std::to_string(player[i]->GetRanking()) + player[i]->GetOrderIndicator()[player[i]->GetRanking() - 1]);
					if(i < Locator::getRM()->player_amount)
						m_cam[i]->SetType(CameraType::ORBIT);
					player[i]->setGamePad(false);
					finished++;
				}
				else if (i < Locator::getRM()->player_amount && player[i]->GetLap() == 2 && !final_lap)
				{
					if (!Locator::getRM()->attract_state)
					{
						Locator::getAudio()->Stop("TRACK_LOOP");
						Locator::getAudio()->Play("FINAL_LAP_SOUND");
					}
					final_lap = true;
					final_lap_start = true;
					timeout = 3.8f;
				}
				player[i]->SetLap(player[i]->GetLap() + 1);

				//Update lap UI for non-AI players
				if (i < Locator::getRM()->player_amount) {
					m_game_ui[i]->SetCurrentLap(player[i]->GetLap());
				}
			}
		}
	}
}

/* Set the player's current position in the race */
void GameScene::SetPlayerRanking()
{
	//Non-AI players, capture current rank to detect an update
	for (int i = 0; i < Locator::getRM()->player_amount; i++) 
	{
		player_rank_save[i] = player[i]->GetRanking();
		player[i]->SetRanking(1);
	}
	for (int i = Locator::getRM()->player_amount; i < m_maxPlayers; i++)
	{
		player[i]->SetRanking(1);
	}

	//Check for current position
	for (int i = 0; i < m_maxPlayers; i++)
	{
		for (int j = 0; j < m_maxPlayers; j++)
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

	//Update UI if position changed
	for (int i = 0; i < Locator::getRM()->player_amount; i++)
	{
		if (player_rank_save[i] != player[i]->GetRanking()) {
			m_game_ui[i]->SetPlayerPosition(player[i]->GetRanking());
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
		loadItemDebugCollider(banana);
		return banana;
	}
	case GREEN_SHELL:
	{
		GreenShell* greenShell = new GreenShell();
		m_itemModels.push_back(greenShell);
		loadItemDebugCollider(greenShell);
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
		using namespace std::placeholders;
		Bomb* bomb = new Bomb(std::bind(&GameScene::CreateExplosion, this, _1));
		m_itemModels.push_back(bomb);
		loadItemDebugCollider(bomb);
		return bomb;
	}
	case FAKE_BOX:
	{
		FakeItemBox* box = new FakeItemBox();
		m_itemModels.push_back(box);
		loadItemDebugCollider(box);
		return box;
	}
	case MUSHROOM_UNLIMITED:
	{
		GoldenMushroom * mushroom = new GoldenMushroom();
		m_itemModels.push_back(mushroom);
		return mushroom;
	}
	case STAR:
	{
		Star* star = new Star();
		m_itemModels.push_back(star);
		return star;
	}
	case MUSHROOM_GIANT:
	{
		GiantMushroom* mushroom = new GiantMushroom();
		m_itemModels.push_back(mushroom);
		return mushroom;
	}
	case LIGHTNING_CLOUD:
	{
		LightningCloud* cloud = new LightningCloud();
		m_itemModels.push_back(cloud);
		loadItemDebugCollider(cloud);
		return cloud;
	}
	case RED_SHELL:
	{
		RedShell* redShell = new RedShell(GetPlayers());
		m_itemModels.push_back(redShell);
		loadItemDebugCollider(redShell);
		return redShell;
	}
	case BULLET_BILL:
	{
		BulletBill* bullet = new BulletBill();
		m_itemModels.push_back(bullet);
		loadItemDebugCollider(bullet);
		return bullet;
	}
	case POW:
	{
		Pow* pow = new Pow(GetPlayers());
		m_itemModels.push_back(pow);
		loadItemDebugCollider(pow);
		return pow;
	}
	case LIGHTNING_BOLT:
	{
		LightningBolt* lightning = new LightningBolt(GetPlayers());
		m_itemModels.push_back(lightning);
		loadItemDebugCollider(lightning);
		return lightning;
	}
	default:
		return nullptr;
	}
}

void GameScene::loadItemDebugCollider(Item* item)
{
#ifdef DEBUG
	m_3DObjects.push_back(dynamic_cast<PhysModel*>(item->GetMesh())->getDebugCollider());
	item->GetMesh()->getDebugCollider()->Load();
#endif DEBUG
}

/* Create an explosion! */
Explosion * GameScene::CreateExplosion(ItemType _ownerType)
{
	Explosion* explosion = new Explosion(_ownerType);
	m_3DObjects.push_back(explosion);
	m_physModels.push_back(explosion);
#ifdef DEBUG
	m_3DObjects.push_back(dynamic_cast<PhysModel*>(explosion)->getDebugCollider());
	explosion->getDebugCollider()->Load();
#endif DEBUG
	return explosion;
}

/* Delete item */
void GameScene::DeleteItem(Item * item)
{
	item->GetMesh()->Reset();
}
void GameScene::DeleteThread(Item * item)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	delete item;
}