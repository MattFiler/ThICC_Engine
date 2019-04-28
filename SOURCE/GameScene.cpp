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
#include "GameObjectShared.h"
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
	for (int i = 0; i < game_config["player_count"]; i++)
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

	return true;
}

/* Populate the expensive things! */
void GameScene::ExpensiveLoad() {
	//Set cubemaps
	Locator::getRD()->current_cubemap_radiance = map_info->cubemap_radiance;
	Locator::getRD()->current_cubemap_irradiance = map_info->cubemap_irradiance;
	Locator::getRD()->current_cubemap_skybox = map_info->cubemap_skybox;

	//Update characters
	for (int i = 0; i < game_config["player_count"]; i++)
	{
		player[i]->Reload(
			Locator::getGOS()->character_instances.at(Locator::getGSD()->character_selected[i]),
			Locator::getGOS()->vehicle_instances.at(Locator::getGSD()->vehicle_selected[i])
		);
	}
	for (int i = game_config["player_count"]; i < 12; i++)
	{
		player[i]->Reload(
			Locator::getGOS()->character_instances.at(Locator::getGSD()->character_selected[0]),
			Locator::getGOS()->vehicle_instances.at(Locator::getGSD()->vehicle_selected[0])
		);
	}

	Locator::getAudio()->addToSoundsList(map_info->audio_background_start, SoundType::GAME);
	Locator::getAudio()->addToSoundsList(map_info->audio_background, SoundType::GAME);
	Locator::getAudio()->addToSoundsList(map_info->audio_final_lap_start, SoundType::GAME);
	Locator::getAudio()->addToSoundsList(map_info->audio_final_lap, SoundType::GAME);

	//Load in
	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		//Load meshes
		(*it)->Load();
		if (dynamic_cast<Player*>(*it)) {
			dynamic_cast<Player*>(*it)->ExpensiveLoad();
		}
		//Load collision info
		if (dynamic_cast<Track*>(*it)) {
			dynamic_cast<Track*>(*it)->LoadCollision();
		}
	}



	//Set AI to current track
	Locator::getAIScheduler()->UpdateTrack(track);

	//Reset key presses
	m_keybinds.Reset();

	//Load skybox
	for (int i = 0; i < game_config["player_count"]; i++) {
		Locator::getRD()->skybox[i]->Load();
	}

	//Load the map's audio here using map_info's data
}

/* Unpopulate the expensive things. */
void GameScene::ExpensiveUnload() {
	Vector3 suitable_spawn = Vector3(0, 0, 0);

	//Unload meshes
	for (std::vector<GameObject3D *>::iterator it = m_3DObjects.begin(); it != m_3DObjects.end(); it++)
	{
		(*it)->Reset();
		if (dynamic_cast<Track*>(*it)) {
			dynamic_cast<Track*>(*it)->UnloadCollision();
			suitable_spawn = dynamic_cast<Track*>(*it)->getSuitableSpawnSpot();
		}
	}

	//Reset player positions & camera mode
	for (int i = 0; i < game_config["player_count"]; i++) {
		player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		m_cam[i]->Reset();
		m_cam[i]->SetType(CameraType::ORBIT);
	}
	for (int i = game_config["player_count"]; i < 12; i++) {
		player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
	}

	cine_cam->Reset();
	cine_cam->SetType(CameraType::CINEMATIC);

	Locator::getAudio()->clearSoundsList(SoundType::GAME);

	//We'll probably need to reset more stuff here, like race timers, etc
	timeout = 12.f;
	state = START;
	m_playerControls = false;
	track_music_start = true;
	final_lap_start = false;
	final_lap = false;
	finished = 0;
	is_paused = false;

	//Unload skybox
	for (int i = 0; i < game_config["player_count"]; i++) {
		Locator::getRD()->skybox[i]->Reset();
	}
}

/* Create all 2D objects for the scene */
void GameScene::create2DObjects()
{
	m_pause_screen = new ImageGO2D("paused");

	for (int i = 0; i < game_config["player_count"]; i++)
	{
		//player[i]->GetItemImg()->SetPos(Vector2(Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX, Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY));
		player[i]->SetItemPos(Vector2(Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX, Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY)); //PART OF THE GROSS MEMORY LEAK
		m_2DObjects.push_back(player[i]->GetItemImg());

		//player[i] = new Text2D(m_localiser.getString(std::to_string(player[i]->getCurrentWaypoint())), _RD);
		float text_pos_x = Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX + Locator::getRD()->m_screenViewportSplitscreen[i].Width - player[i]->GetRankingText()->GetSize().x * 2.f;
		float text_pos_y = Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY + Locator::getRD()->m_screenViewportSplitscreen[i].Height - player[i]->GetRankingText()->GetSize().y;
		player[i]->GetRankingText()->SetPos(Vector2(text_pos_x, text_pos_y));
		//m_2DObjects.push_back(player[i]->GetRankingText());

		float text_lap_x = Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX + player[i]->GetLapText()->GetSize().x * 0.25f;
		float text_lap_y = Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY + Locator::getRD()->m_screenViewportSplitscreen[i].Height - player[i]->GetLapText()->GetSize().y;
		player[i]->GetLapText()->SetPos(Vector2(text_lap_x, text_lap_y));
		//m_2DObjects.push_back(player[i]->GetLapText());
	}


	// player countdown text
	for (int i = 0; i < game_config["player_count"]; i++)
	{
		player[i]->GetCountdown()->SetPos({
			Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX + Locator::getRD()->m_screenViewportSplitscreen[i].Width / 2 - player[i]->GetCountdown()->GetSize().x / 2 ,
			Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY + Locator::getRD()->m_screenViewportSplitscreen[i].Height / 2 - player[i]->GetCountdown()->GetSize().y / 2 
		});
		player[i]->GetFinishOrder()->SetPos({ 
			Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftX + Locator::getRD()->m_screenViewportSplitscreen[i].Width / 2 - player[i]->GetFinishOrder()->GetSize().x / 2 ,
			Locator::getRD()->m_screenViewportSplitscreen[i].TopLeftY + Locator::getRD()->m_screenViewportSplitscreen[i].Height / 2 - player[i]->GetFinishOrder()->GetSize().y / 2
		});
	}
}

/* Create all 3D objects in the scene. */
void GameScene::create3DObjects()
{
	//Load in a track
	track = new Track(map_info->model);
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

	DebugText::print("Width: " + std::to_string(Locator::getRD()->m_window_width));
	DebugText::print("Height: " + std::to_string(Locator::getRD()->m_window_height));

	Vector3 suitable_spawn = track->getSuitableSpawnSpot();
	for (int i = 0; i < game_config["player_count"]; i++) {

		//Create a player and position on track
		using std::placeholders::_1;
		player[i] = new Player(
			Locator::getGOS()->character_instances.at(Locator::getGSD()->character_selected[i]),
			Locator::getGOS()->vehicle_instances.at(Locator::getGSD()->vehicle_selected[i]),
			i, std::bind(&GameScene::CreateItem, this, _1)
		);
		player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		player[i]->setMass(10);
		m_3DObjects.push_back(player[i]);

		//Create a camera to follow the player
		m_cam[i] = new Camera(Locator::getRD()->m_window_width, Locator::getRD()->m_window_height, Vector3(0.0f, 3.0f, 10.0f), player[i], CameraType::FOLLOW);
		m_cam[i]->setAngle(180.0f);
	}

	// Spawn in the AI
	for (int i = game_config["player_count"]; i < 12; i++) {

		//Create a player and position on track
		using std::placeholders::_1;
		player[i] = new Player(
			Locator::getGOS()->character_instances.at(Locator::getGSD()->character_selected[0]),
			Locator::getGOS()->vehicle_instances.at(Locator::getGSD()->vehicle_selected[0]),
			-1, std::bind(&GameScene::CreateItem, this, _1)
		);
		player[i]->SetPos(Vector3(suitable_spawn.x, suitable_spawn.y, suitable_spawn.z - (i * 10)));
		player[i]->setMass(10);
		m_3DObjects.push_back(player[i]);
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

	if (Locator::getID()->m_keyboardTracker.pressed.N) {
		Locator::getID()->TEST++;
		DebugText::print(std::to_string(Locator::getID()->TEST));
	}
	if (Locator::getID()->m_keyboardTracker.pressed.B) {
		Locator::getID()->TEST--;
		DebugText::print(std::to_string(Locator::getID()->TEST));
	}

	//camera_pos->SetText(std::to_string((int)cine_cam->GetPos().x) + "," + std::to_string((int)cine_cam->GetPos().y) + "," + std::to_string((int)cine_cam->GetPos().z));

	Locator::getAIScheduler()->Update();

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
		Locator::getAudio()->GetSound(SoundType::MISC, (int)MiscSounds::INTRO_MUSIC)->SetVolume(1.f);
		Locator::getAudio()->Play(SoundType::MISC, (int)MiscSounds::INTRO_MUSIC);
		state = OPENING;
		break;
	case OPENING:
		if (timeout >= 0)
		{
			timeout -= Locator::getGSD()->m_dt;
			cine_cam->Tick();
			Locator::getRD()->skybox[0]->Tick(cine_cam);
			if (timeout <= Locator::getGSD()->m_dt + 0.1) {
				for (int i = 0; i < game_config["player_count"]; ++i) {
					m_cam[i]->Tick(); 
					Locator::getRD()->skybox[i]->Tick(m_cam[i]);
				}
			}
		}
		else
		{
			for (int i = 0; i < game_config["player_count"]; ++i) {
				m_cam[i]->Tick();
				Locator::getRD()->skybox[i]->Tick(m_cam[i]);
			}
			state = CAM_OPEN;
			timeout = 2.99999f;
			Locator::getAudio()->Play(SoundType::MISC, (int)MiscSounds::PRE_COUNTDOWN);
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
		for (int i = 0; i < game_config["player_count"]; ++i) {
			m_cam[i]->Tick();
			Locator::getRD()->skybox[i]->Tick(m_cam[i]);
		}
		cine_cam->Tick();

		if (m_cam[game_config["player_count"]-1]->GetType() == CameraType::FOLLOW)
		{
			Locator::getAudio()->GetSound(SoundType::MISC, (int)MiscSounds::COUNTDOWN)->SetVolume(0.7f);
			Locator::getAudio()->Play(SoundType::MISC, (int)MiscSounds::COUNTDOWN);
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
			Locator::getAudio()->GetSound(SoundType::GAME, (int)GameSounds::MKS_START)->SetVolume(0.7f);
			Locator::getAudio()->Play(SoundType::GAME, (int)GameSounds::MKS_START);
			for (int i = 0; i < 12; ++i)
			{
				player[i]->setGamePad(true);
			}

		}
		break;
	case PLAY:
		for (int i = 0; i < game_config["player_count"]; ++i) {
			m_cam[i]->Tick();
			Locator::getRD()->skybox[i]->Tick(m_cam[i]);
		}

		timeout -= Locator::getGSD()->m_dt;

		if (timeout <= 0 && track_music_start)
		{
			Locator::getAudio()->Play(SoundType::GAME, (int)GameSounds::MKS_GAME);
			track_music_start = false;
		}

		if (timeout <= 0 && final_lap_start)
		{
			Locator::getAudio()->Play(SoundType::GAME, (int)GameSounds::MKS_FL_GAME);
			final_lap_start = false;
		}

		break;
	}

	for (int i = 0; i < 12; ++i) {
		player[i]->ShouldStickToTrack(*track);
		//player[i]->ResolveWallCollisions(*track);
	}

	if (m_keybinds.keyReleased("Quit"))
	{
		Locator::getAudio()->GetSound(SoundType::GAME, (int)GameSounds::MKS_GAME)->Stop();
		Locator::getAudio()->GetSound(SoundType::GAME, (int)GameSounds::MKS_FL_GAME)->Stop();
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
	if (m_keybinds.keyReleased("toggle collision debug"))
	{
		GameDebugToggles::show_debug_meshes = !GameDebugToggles::show_debug_meshes;
		DebugText::print("show_debug_meshes: " + std::to_string((int)GameDebugToggles::show_debug_meshes));
	}
	if (m_keybinds.keyReleased("toggle world render")) {
		GameDebugToggles::render_level = !GameDebugToggles::render_level;
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
				else
				{
					(*it)->Render();
				}
			}
		}

		//Render items
		for (Item* obj : m_itemModels)
		{
			obj->Render();
		}

		break;
	case CAM_OPEN:
	case COUNTDOWN:
		for (int i = 0; i < game_config["player_count"]; ++i)
		{
			//Configure viewports
			m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewportSplitscreen[i]);
			m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRectSplitscreen[i]);
			Locator::getRD()->m_cam = m_cam[i];

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
					obj->Render();
				}
			}
		}
		break;
	case PLAY:
		for (int i = 0; i < game_config["player_count"]; ++i)
		{
			//Setup viewport
			m_commandList->RSSetViewports(1, &Locator::getRD()->m_screenViewportSplitscreen[i]);
			m_commandList->RSSetScissorRects(1, &Locator::getRD()->m_scissorRectSplitscreen[i]);
			Locator::getRD()->m_cam = m_cam[i];

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
					obj->Render();
				}
			}
		}
		break;
	}

	// Render skyboxes that are loaded
	
	/*SKYBOXES ARE DISABLED FOR NOW DUE TO A RENDERING BUG
	for (int i = 0; i < game_config["player_count"]; i++) {
		if (Locator::getRD()->skybox[i]->Loaded()) {
			Locator::getRD()->skybox[i]->Render();
		}
	}*/
}

/* Render the 2D scene */
void GameScene::Render2D(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>&  m_commandList)
{
	if (is_paused) {
		m_pause_screen->Render();
		return;
	}

	switch (state)
	{
	case COUNTDOWN:
		//Render countdown in screen centre
		for (int i = 0; i < game_config["player_count"]; i++)
		{
			player[i]->GetCountdown()->Render();
		}
		break;
	case PLAY:
		/*
		for (std::vector<GameObject2D *>::iterator it = m_2DObjects.begin(); it != m_2DObjects.end(); it++)
		{
			(*it)->Render();
		}*/

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
		break;
	}
}

/* Set the player's current waypoint */
void GameScene::SetPlayersWaypoint()
{
	Vector3 currentWaypoint;
	Vector3 nextWaypoint;
	for (int i = 0; i < 12; i++) {
		currentWaypoint = track->getWaypointMiddle(player[i]->GetWaypoint());
		int nextWayIndex = player[i]->GetWaypoint() + 1;
		if (nextWayIndex == track->getWaypoints().size())
		{
			nextWayIndex = 0;
		}
		nextWaypoint = track->getWaypointMiddle(nextWayIndex);
		bool switchWaypoint = Vector3::Distance(player[i]->GetPos(), nextWaypoint) < Vector3::Distance(currentWaypoint, nextWaypoint);
		if (switchWaypoint || player[i]->GetWaypoint() < track->getWaypointsBB().size() - 1)
		{
			if (player[i]->getCollider().Intersects(track->getWaypointsBB()[player[i]->GetWaypoint() + 1]))
			{
				player[i]->SetWaypoint(player[i]->GetWaypoint() + 1);
			}
		}
		else
		{
			if (switchWaypoint || player[i]->getCollider().Intersects(track->getWaypointsBB()[0]))
			{
				player[i]->SetWaypoint(0);
				if (player[i]->GetLap() == 3)
				{
					player[i]->SetFinished(true);
					player[i]->GetFinishOrder()->SetText(std::to_string(player[i]->GetRanking()) + player[i]->GetOrderIndicator()[player[i]->GetRanking() - 1]);
					if(i < game_config["player_count"])
						m_cam[i]->SetType(CameraType::ORBIT);
					player[i]->setGamePad(false);
					finished++;
				}
				else if (i < game_config["player_count"] && player[i]->GetLap() == 2 && !final_lap)
				{
					Locator::getAudio()->GetSound(SoundType::GAME, (int)GameSounds::MKS_GAME)->Stop();
					Locator::getAudio()->Play(SoundType::MISC, (int)MiscSounds::FINAL_LAP_IND);
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
	for (int i = 0; i < 12; i++)
	{
		player[i]->SetRanking(1);
	}

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
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
		banana->GetMesh()->getDebugCollider()->Load();
		return banana;
	}
	case GREEN_SHELL:
	{
		GreenShell* greenShell = new GreenShell();
		m_itemModels.push_back(greenShell);
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(greenShell->GetMesh())->getDebugCollider());
		greenShell->GetMesh()->getDebugCollider()->Load();

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
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(bomb->GetMesh())->getDebugCollider());
		bomb->GetMesh()->getDebugCollider()->Load();

		return bomb;
	}
	case FAKE_BOX:
	{
		FakeItemBox* box = new FakeItemBox();
		m_itemModels.push_back(box);
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(box->GetMesh())->getDebugCollider());
		box->GetMesh()->getDebugCollider()->Load();

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
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(cloud->GetMesh())->getDebugCollider());
		cloud->GetMesh()->getDebugCollider()->Load();
		return cloud;
	}
	case RED_SHELL:
	{
		RedShell* redShell = new RedShell();
		m_itemModels.push_back(redShell);
		m_3DObjects.push_back(dynamic_cast<PhysModel*>(redShell->GetMesh())->getDebugCollider());
		redShell->GetMesh()->getDebugCollider()->Load();

		return redShell;
	}
	default:
		return nullptr;
	}
}

/* Create an explosion! */
Explosion * GameScene::CreateExplosion(ItemType _ownerType)
{
	Explosion* explosion = new Explosion(_ownerType);
	m_3DObjects.push_back(explosion);
	m_physModels.push_back(explosion);
	m_3DObjects.push_back(dynamic_cast<PhysModel*>(explosion)->getDebugCollider());
	explosion->getDebugCollider()->Load();

	return explosion;
}

/* Delete item (this is a mixup of old and new stuff - for the record, you just need to call Reset() on models to delete them - none of this threading crap.) */
void GameScene::DeleteItem(Item * item)
{
	item->GetMesh()->Reset();
	/*
	std::thread thread(&GameScene::DeleteThread, this, item);
	thread.detach();*/
}
void GameScene::DeleteThread(Item * item)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	delete item;
}