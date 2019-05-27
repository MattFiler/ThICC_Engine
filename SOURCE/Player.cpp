#include "pch.h"
#include "Player.h"
#include "GameStateData.h"
#include "ServiceLocator.h"
#include "InputData.h"
#include "ItemData.h"
#include "AIScheduler.h"
#include "GameObjectShared.h"
#include "KartAI.h"
#include <iostream>
#include <fstream>

extern void ExitGame();

Player::Player(CharacterInfo* _character, VehicleInfo* _vehicle, int _playerID, std::function<Item*(ItemType)> _createItemFunction) : TrackMagnet(_character->model), CreateItem(_createItemFunction)
{
	InitPlayerData();

	m_RD = Locator::getRD();
	SetDrag(0.7);
	m_useGroundTypes = true;
	SetPhysicsOn(true);
	m_playerID = _playerID;

	//TODO: Move this all to InGameUI
	m_textCountdown = new Text2D("3", Text2D::MIDDLE);
	m_imgItem = Locator::getItemData()->GetItemSprite(PLACEHOLDER, m_playerID);
	m_textFinishOrder = new Text2D("0" + m_orderIndicator[0]);

	// Don't render this mesh, render a second one instead
	m_shouldRender = false;

	m_move = std::make_unique<ControlledMovement>(this, m_animationMesh.get());

	// If AI
	if (m_playerID == -1)
	{
		m_move->SetEnabled(false);
	}
}

void Player::InitPlayerData()
{
	std::ifstream i(m_filepath.generateConfigFilepath("PLAYER_CONFIG", m_filepath.CONFIG));
	json playerData;
	playerData << i;

	m_maxItems = (float)playerData["item_data"]["triple_item_count"];
	m_firstTrailingItemOffset = (float)playerData["item_data"]["trailing_items"]["line"]["first_item_trail_offset"];
	m_otherTrailingItemOffset = (float)playerData["item_data"]["trailing_items"]["line"]["other_items_trail_offset"];
	m_orbitDistance = Vector3((float)playerData["item_data"]["trailing_items"]["spinning"]["orbit_distance"][0],
		(float)playerData["item_data"]["trailing_items"]["spinning"]["orbit_distance"][1],
		(float)playerData["item_data"]["trailing_items"]["spinning"]["orbit_distance"][2]);
	m_orbitSpeed = (float)playerData["item_data"]["trailing_items"]["spinning"]["orbit_speed"];
	m_floatingItemPosOffset = (float)playerData["item_data"]["trailing_items"]["floating"]["upward_pos_offset"];

	m_posHistoryInterval = (float)playerData["respawn_data"]["pos_history_interval"];
	m_posHistoryTimer = (float)playerData["respawn_data"]["pos_history_timer"];
	m_posHistoryLength = (float)playerData["respawn_data"]["pos_history_length"];
	m_respawnDelay = (float)playerData["respawn_data"]["respawn_delay"];
}

Player::~Player()
{
	delete m_imgItem;
	m_imgItem = nullptr;
}

void Player::SetPlayerID(int val) 
{ 
	m_playerID = val;
}

void Player::Reload(CharacterInfo* _character, VehicleInfo* _vehicle) {

	std::ifstream i(m_filepath.generateConfigFilepath(_vehicle->model, m_filepath.MODEL));
	json m_model_config_vehicle;
	m_model_config_vehicle << i;

	m_animationMesh = std::make_unique<AnimationController>();
	SetScale(m_model_config_vehicle["modelscale"]);

	m_animationMesh->AddModel("vehicle", _vehicle->model);
	m_animationMesh->AddModel("character", _character->model);
	m_animationMesh->AddModel("lakitu", Locator::getGOS()->common_model_config["referee"]);
	m_animationMesh->AddModel("glider", Locator::getGOS()->common_model_config["glider"]);
	m_animationMesh->AddModel("Bullet Bill", Locator::getItemData()->GetItemModelName(BULLET_BILL));
	m_animationMesh->AddModel("Cloud", Locator::getItemData()->GetItemModelName(LIGHTNING_CLOUD));
	m_animationMesh->AddModel("POW", Locator::getItemData()->GetItemModelName(POW));

	m_animationMesh->AddModelSet("default", std::vector < std::string>{"vehicle", "character"});
	m_animationMesh->AddModelSet("respawn", std::vector < std::string>{"vehicle", "character", "lakitu"});
	m_animationMesh->AddModelSet("gliding", std::vector < std::string>{"vehicle", "character", "glider"});
	m_animationMesh->AddModelSet("Bullet Bill", std::vector < std::string>{"Bullet Bill"});
	m_animationMesh->AddModelSet("Cloud", std::vector < std::string>{"vehicle", "character","Cloud"});
	m_animationMesh->AddModelSet("Cloud Respawn", std::vector < std::string>{"vehicle", "character", "Cloud", "lakitu"});
	m_animationMesh->AddModelSet("Cloud Gliding", std::vector < std::string>{"vehicle", "character", "Cloud", "glider"});	
	m_animationMesh->AddModelSet("POW", std::vector < std::string>{"vehicle", "character","POW"});
	m_animationMesh->AddModelSet("POW Respawn", std::vector < std::string>{"vehicle", "character", "POW", "lakitu"});
	m_animationMesh->AddModelSet("POW Gliding", std::vector < std::string>{"vehicle", "character", "POW", "glider"});	
	m_animationMesh->AddModelSet("Cloud POW", std::vector < std::string>{"vehicle", "character", "Cloud", "POW"});
	m_animationMesh->AddModelSet("Cloud POW Respawn", std::vector < std::string>{"vehicle", "character","Cloud", "POW", "lakitu"});
	m_animationMesh->AddModelSet("Cloud POW Gliding", std::vector < std::string>{"vehicle", "character","Cloud", "POW", "glider"});

	m_animationMesh->SwitchModelSet("default");

	m_animationMesh->Load();

	ControlledMovement* old_movement = m_move.release();
	m_move = std::make_unique<ControlledMovement>(this, m_animationMesh.get());

	m_normalGrav = m_maxGrav;

	m_lastFramePos = m_pos;
	//Update TrackMagnet here too?
}

void Player::SetActiveItem(ItemType _item)
{
	if (m_InventoryItem == _item) 
	{
		if (_item != LIGHTNING_CLOUD)
		{
			active_item = _item;
			m_imgItem = Locator::getItemData()->GetItemSprite(PLACEHOLDER, m_playerID);
			m_imgItem->SetPos(m_itemPos);
			m_imgItem->SetScale(m_itemScale);
			did_use_item = true;
		}

		m_InventoryItem = ItemType::NONE;
		
		DebugText::print("PLAYER " + std::to_string(m_playerID) + " HAS ACTIVATED ITEM: " + std::to_string(_item));
	}
	else
	{
		//We should never often get here
		DebugText::print("PLAYER TRIED TO USE AN ITEM THEY DON'T HAVE - THIS SHOULD NEVER BE REQUESTED!");
		DebugText::print("UNLESS THEY'RE MAKING A TRIPLE ITEM");
	}
}

void Player::SetItemInInventory(ItemType _item) {
	if (m_InventoryItem == ItemType::NONE) {
		m_InventoryItem = _item;

		if (m_InventoryItem == LIGHTNING_CLOUD && GetLightningCloud())
		{
			m_InventoryItem = BANANA;
		}

		if (m_playerID != -1)
		{
			m_imgItem = Locator::getItemData()->GetItemSprite(m_InventoryItem, m_playerID);
			m_imgItem->SetPos(m_itemPos);
			m_imgItem->SetScale(m_itemScale);
		}
		DebugText::print("PLAYER " + std::to_string(m_playerID) + " HAS ACQUIRED ITEM: " + std::to_string(_item));

		//Lightning cloud spawns as soon as it gets picked up
		if (m_InventoryItem == LIGHTNING_CLOUD)
		{
			SpawnItems(LIGHTNING_CLOUD);
		}
	}
}

LightningCloud* Player::GetLightningCloud()
{
	for (Item* item : m_floatingItems)
	{
		LightningCloud* cloud = dynamic_cast<LightningCloud*>(item);
		if (cloud)
		{
			return cloud;
		}
	}

	return nullptr;
}

void Player::RemoveLightningCloud()
{
	for (int i = 0; i < m_floatingItems.size(); ++i)
	{
		if (dynamic_cast<LightningCloud*>(m_floatingItems[i]))
		{
			m_floatingItems.erase(m_floatingItems.begin() + i);
			return;
		}
	}
}

void Player::RemoveLightningCloudModel()
{
	if (IsGliding())
	{
		m_animationMesh->SwitchModelSet("gliding");
	}
	else if (IsRespawning())
	{
		m_animationMesh->SwitchModelSet("respawn");
	}
	else
	{
		m_animationMesh->SwitchModelSet("default");
	}
}

void Player::Render()
{
	if (!m_animationMesh)
		return;

	m_animationMesh->Render();
	TrackMagnet::Render();

	/*if (m_ai)
	{
		m_ai->DebugRender();
	}*/
}


void Player::Tick()
{
	if (!m_animationMesh)
		return;

	movement();

	RespawnLogic();

	GlideLogic();

	if (m_controlsActive)
	{
		CheckUseItem();
	}
	
	// Debug code to save/load the players game state
	if (m_keybind.keyReleased("debug save position"))
	{
		m_savedMatrix = m_world;
		m_savedVel = m_vel;
		m_savedGravVel = m_gravVel;
		m_savedGravDir = m_gravDirection;
	}
	else if (m_keybind.keyReleased("debug load position"))
	{
		SetWorld(m_savedMatrix);
		m_vel = m_savedVel;
		m_gravVel = m_savedGravVel;
		m_velTotal = m_vel + m_savedGravVel;
		m_gravDirection = m_savedGravDir;
	}
	/*else
	{
		ReleaseItem();
	}*/

	//Debug output player location - useful for setting up spawns
	if (m_keybind.keyReleased("Print Player Location")) {
		DebugText::print("PLAYER POSITION: (" + std::to_string(m_pos.x) + ", " + std::to_string(m_pos.y) + ", " + std::to_string(m_pos.z) + ")");
	}

	//Update UI (TODO: move to InGameUI)
	if (!m_finished)
	{
		m_imgItem->Tick();
	}

	//apply my base behaviour
	TrackMagnet::Tick();

	m_animationMesh->Update(m_world);

	m_lastFramePos = m_pos;
}

void Player::CheckUseItem()
{
	if (m_ai)
	{
		return;
	}
	if (m_multiItem)
	{
		TrailItems();

		if (m_keybind.keyHeld("activate", m_playerID))
		{
			if (m_trailingItems.empty() && m_InventoryItem != NONE)
			{
				SpawnItems(m_InventoryItem);
			}
			else if (!m_aPressed)
			{
				ReleaseItem();
			}
			
			m_aPressed = true;
		}
		else
		{
			m_aPressed = false;
		}
	}
	else
	{
		if (m_keybind.keyHeld("activate", m_playerID))
		{
			if (m_trailingItems.empty() && m_InventoryItem != NONE && !m_aPressed)
			{
				SpawnItems(m_InventoryItem);
			}
			else
			{
				TrailItems();
			}

			m_aPressed = true;
		}
		else
		{
			ReleaseItem();
			m_aPressed = false;
		}
	} 
}

void Player::TrailItems()
{
	if (!m_dropItems)
	{
		if (!m_trailingItems.empty())
		{
			DebugText::print("Hit");
			for (int i = 0; i < m_trailingItems.size(); i++)
			{
				if (m_trailingItems[i]->ShouldDestroy())
				{
					m_trailingItems.erase(m_trailingItems.begin() + i);
					if (m_InventoryItem == MUSHROOM_UNLIMITED) {
						SetActiveItem(MUSHROOM_UNLIMITED);
						active_item = NONE;
					}
					continue;
				}

				if (m_trailingItems[i]->GetItemMesh())
				{
					if (m_trailingItems[i]->ShouldDestroy())
					{
						m_trailingItems.erase(m_trailingItems.begin() + i);
						continue;
					}

					//Trails behind the player
					if (active_item != GREEN_SHELL_3X && active_item != RED_SHELL_3X)
					{
						Vector3 backward_pos = i > 0 ? m_trailingItems[i - 1]->GetMesh()->GetWorld().Backward() : m_world.Backward();

						m_trailingItems[i]->GetMesh()->SetWorld(m_world);
						m_trailingItems[i]->GetMesh()->AddPos(backward_pos * m_firstTrailingItemOffset + (backward_pos * m_otherTrailingItemOffset * i));
						m_trailingItems[i]->GetMesh()->UpdateWorld();
					}
					//Spins around the player
					else
					{
						m_trailingItems[i]->GetMesh()->SetWorld(m_world);
						m_trailingItems[i]->setSpinAngle(m_trailingItems[i]->getSpinAngle() + m_orbitSpeed * Locator::getGSD()->m_dt);
						m_trailingItems[i]->GetMesh()->AddPos(Vector3::Transform({ sin(m_trailingItems[i]->getSpinAngle() / 57.2958f)
							* m_orbitDistance.x, m_orbitDistance.y, cos(m_trailingItems[i]->getSpinAngle() / 57.2958f) * m_orbitDistance.z }, m_rot));
					}
				}

				m_trailingItems[i]->setTrailing(true);
			}
		}
	}
	else
	{
		for (int i = 0; i < m_trailingItems.size(); i++)
		{
			if (m_trailingItems[i]->GetItemMesh())
			{
				m_trailingItems[i]->setItemInUse(this);
				m_trailingItems.erase(m_trailingItems.begin() + i);
			}
		}
		m_dropItems = false;
	}
}

void Player::SpawnItems(ItemType type)
{
	//Triple mushrooms, Golden Mushroom and Bullet Bill still in inventory after use
	if (type != MUSHROOM_3X && type != MUSHROOM_UNLIMITED && type != BULLET_BILL)
	{
		SetActiveItem(type);
	}

	switch (type)
	{
		case BANANA:
		{
			Banana * banana = static_cast<Banana*>(CreateItem(BANANA));
			m_trailingItems.push_back(banana);
			TrailItems();
			break;
		}

		case MUSHROOM:
		{
			Mushroom* mushroom = static_cast<Mushroom*>(CreateItem(MUSHROOM));
			mushroom->Use(this, false);
			break;
		}

		case GREEN_SHELL:
		{
			GreenShell* shell = static_cast<GreenShell*>(CreateItem(GREEN_SHELL));
			m_trailingItems.push_back(shell);
			TrailItems();
			break;
		}

		case BOMB:
		{
			Bomb* bomb = static_cast<Bomb*>(CreateItem(BOMB));
			m_trailingItems.push_back(bomb);
			TrailItems();
			break;
		}

		case BANANA_3X:
		{
			for (int i = 0; i < m_maxItems; i++)
			{
				SpawnItems(BANANA);
			}

			for (Item*& banana : m_trailingItems)
			{
				banana->addImmuneItems(m_trailingItems);
			}

			m_multiItem = true;
			break;
		}

		case MUSHROOM_3X:
		{
			//uses the first shroom
			SpawnItems(MUSHROOM);

			//creates subsequence shrooms
			for (int i = 0; i < m_maxItems - 1; i++)
			{
				Mushroom* mushroom = static_cast<Mushroom*>(CreateItem(MUSHROOM));
				m_trailingItems.push_back(mushroom);
			}

			m_multiItem = true;
			break;
		}

		case GREEN_SHELL_3X:
		{
			for (int i = 0; i < m_maxItems; i++)
			{
				SpawnItems(GREEN_SHELL);
			}

			for (int i = 0; i < m_maxItems; i++)
			{
				m_trailingItems[i]->addImmuneItems(m_trailingItems);
				m_trailingItems[i]->setSpinAngle((360/ m_trailingItems.size()) * i);
			}

			m_multiItem = true;
			break;
		}

		case FAKE_BOX:
		{
			FakeItemBox* box = static_cast<FakeItemBox*>(CreateItem(FAKE_BOX));
			m_trailingItems.push_back(box);
			TrailItems();
			break;
		}

		case MUSHROOM_UNLIMITED:
		{
			GoldenMushroom* mushroom = static_cast<GoldenMushroom*>(CreateItem(MUSHROOM_UNLIMITED));
			mushroom->Use(this, false);
			m_trailingItems.push_back(mushroom);
			m_multiItem = true;
			break;
		}

		case STAR:
		{
			Star* star = static_cast<Star*>(CreateItem(STAR));
			star->Use(this, false);
			break;
		}

		case MUSHROOM_GIANT:
		{
			GiantMushroom* mushroom = static_cast<GiantMushroom*>(CreateItem(MUSHROOM_GIANT));
			mushroom->Use(this, false);
			break;
		}

		case LIGHTNING_CLOUD:
		{
			LightningCloud* cloud = static_cast<LightningCloud*>(CreateItem(LIGHTNING_CLOUD));
			m_floatingItems.push_back(cloud);
			cloud->Use(this, false);
			break;
		}

		case RED_SHELL:
		{
			RedShell* shell = static_cast<RedShell*>(CreateItem(RED_SHELL));
			m_trailingItems.push_back(shell);
			TrailItems();
			break;
		}

		case BULLET_BILL:
		{
			BulletBill* bullet = static_cast<BulletBill*>(CreateItem(BULLET_BILL));
			bullet->Use(this, false);
			break;
		}

		case POW:
		{
			Pow* pow = static_cast<Pow*>(CreateItem(POW));
			pow->Use(this, false);
			break;
		}

		default:
			break;
	}

	for (Item*& item : m_trailingItems)
	{
		item->setPlayer(this);
	}

}

void Player::ReleaseItem()
{
	if (!m_trailingItems.empty())
	{
		m_trailingItems[m_trailingItems.size() - 1]->Use(this, m_keybind.keyHeld("item alternate use", m_playerID));
		m_trailingItems[m_trailingItems.size() - 1]->setTrailing(false);

		if (m_InventoryItem != MUSHROOM_UNLIMITED)
		{
			m_trailingItems.pop_back();
		}

		if (m_trailingItems.empty())
		{
			m_multiItem = false;
			active_item = NONE;

			if (m_InventoryItem == MUSHROOM_3X)
			{
				SetActiveItem(MUSHROOM_3X);
				active_item = NONE;
			}
		}
	}
}

void Player::DropItems()
{
	if (Locator::getItemData()->CanDrop(m_InventoryItem))
	{
		SpawnItems(m_InventoryItem);
	}
	m_dropItems = true;
}

void Player::setGamePad(bool _state)
{
	// If AI
	if (m_playerID == -1 || (m_lap == 3 && !m_ai))
	{
		m_move->SetGamepadActive(false);
		m_ai = std::make_unique<KartAI>(this, m_move.get());
		m_ai->UseDrift(true);
		m_move->SetEnabled(true);
		return;
	}
	m_move->SetGamepadActive(_state);
	m_move->SetPlayerID(m_playerID);
	m_controlsActive = _state;
}

void Player::movement()
{
	// Disable drifting off the track
	m_move->EnableDrifting(!(m_colType == OFF_TRACK || m_colType == GLIDER_TRACK || m_colType == NO_TERRAIN));
	m_move->Tick();

	// Debug code to save/load the players game state
	if (m_keybind.keyReleased("debug save position"))
	{
		m_savedMatrix = m_world;
		m_savedVel = m_vel;
		m_savedGravVel = m_gravVel;
	}
	else if (m_keybind.keyReleased("debug load position"))
	{
		m_world = m_savedMatrix;
		m_vel = m_savedVel;
		m_gravVel = m_savedGravVel;
		Vector3 scale = Vector3::Zero;
		Quaternion rot = Quaternion::Identity;
		m_world.Decompose(scale, rot, m_pos);
		m_rot = Matrix::CreateFromQuaternion(rot);
	}

	//Debug output player location - useful for setting up spawns
	if (m_keybind.keyReleased("print player location")) {
		DebugText::print("PLAYER POSITION: (" + std::to_string(m_pos.x) + ", " + std::to_string(m_pos.y) + ", " + std::to_string(m_pos.z) + ")");
	}

}

void Player::GlideLogic()
{
	if (m_colType == CollisionType::JUMP_PAD)
	{
		m_gliding = true;
		m_animationMesh->SwitchModelSet("gliding");
		m_preventRespawn = true;
		m_move->SetGliding(true);
		m_elapsedTimeOff = 0;
	}
	else if (m_colType == CollisionType::GLIDER_TRACK)
	{
		m_elapsedTimeOff = 0;
		m_maxGrav = 0;
		m_gravVel = Vector3::Zero;
	}
	else if (m_gliding)
	{
		m_glideTimeElapsed += Locator::getGSD()->m_dt;
		m_drag = 0.3f;
		if (m_onTrack && m_glideTimeElapsed > m_minGlideDuration)
		{
			StopGlide();
		}
		else if (m_colType == CollisionType::NO_TERRAIN)
		{
			m_elapsedTimeOff += Locator::getGSD()->m_dt;
			if (m_elapsedTimeOff > m_maxTimeGlidingOff)
			{
				m_preventRespawn = false;
				m_maxGrav = m_glidingGrav;
			}
		}
		else
		{
			m_maxGrav = m_glidingGrav;
		}
	}
}

void Player::StopGlide()
{
	m_elapsedTimeOff = 0;
	m_glideTimeElapsed = 0;
	m_preventRespawn = false;
	m_gliding = false;
	m_maxGrav = m_normalGrav;
	m_move->SetGliding(false);
	m_animationMesh->SwitchModelSet("default");
}

void Player::RespawnLogic()
{
	if (m_respawning)
	{
		MovePlayerToTrack();
		return;
	}

	if (m_preventRespawn)
	{
		return;
	}

	m_timeSinceRespawn += Locator::getGSD()->m_dt;

	if (!m_respawning && m_onTrack && m_colType == CollisionType::ON_TRACK)
	{

		m_posHistoryTimer += Locator::getGSD()->m_dt;
		m_offTrackTimer = 0;
		m_offTerrainTimer = 0;
		if (m_posHistoryTimer >= m_posHistoryInterval && !m_respawning)
		{
			m_posHistoryTimer -= m_posHistoryInterval;
			m_matrixHistory.push(m_world);
			m_posHistory.push(m_pos);

			if (m_matrixHistory.size() > m_posHistoryLength)
			{
				m_matrixHistory.pop();
				m_posHistory.pop();
			}
		}
	}
	else
	{
		if (m_colType == CollisionType::NO_TERRAIN)
		{
			if (m_offTrackTimer >= m_noTrackRespawn)
			{
				Respawn();
				m_offTrackTimer = 0;
			}
			else
			{
				m_offTrackTimer += Locator::getGSD()->m_dt;
			}
		}
		else if (m_colType == CollisionType::OFF_TRACK)
		{
			if (m_offTerrainTimer >= m_offTrackRespawn)
			{
				Respawn();
				m_offTerrainTimer = 0;
			}
			else
			{
				m_offTerrainTimer += Locator::getGSD()->m_dt;
			}
		}
	}
}

Vector3 Player::GetPosHistoryBack()
{
	if (m_posHistory.empty())
	{
		return m_pos;
	}
	else
	{
		return m_posHistory.back();
	}
}

void Player::Respawn()
{
	if (m_matrixHistory.size() == 0) {
		DebugText::print("Player called a respawn but had zero values to assign to m_endWorld. Fatal!");
		return;
	}

	StopGlide();
	m_move->SetEnabled(false);
	m_animationMesh->SwitchModelSet("respawn");
	m_respawning = true;
	m_glideTimeElapsed = 0;
	m_elapsedTimeOff = 0;
	UseMagnet(false);

	m_respawnStartPos = m_pos;
	m_respawnStartRot = m_quatRot;
	m_endWorld = m_matrixHistory.front();

	Vector3 scale;
	m_matrixHistory.front().Decompose(scale, m_respawnEndRot, m_respawnEndPos);
	m_respawnEndPos += m_matrixHistory.front().Up() * (data.m_height * 5);

	// Find the distance to the respawn point
	float dist = Vector3::Distance(m_respawnEndPos, m_pos);
	m_totalRespawnTime = dist / m_respawnSpeed;
	if (m_totalRespawnTime > m_maxRespawnTime)
	{
		m_totalRespawnTime = m_maxRespawnTime;
	}

	m_posHistoryTimer = 0;
	m_vel = Vector::Zero;
	m_gravVel = Vector::Zero;
	m_velTotal = Vector::Zero;
}

void Player::MovePlayerToTrack()
{
	m_elapsedRespawnTime += Locator::getGSD()->m_dt;
	if (m_elapsedRespawnTime > m_totalRespawnTime)
	{
		m_elapsedRespawnTime = 0;
		m_pos = m_respawnEndPos;
		m_rot = Matrix::CreateFromQuaternion(m_respawnEndRot);
		UpdateWorld();
		m_respawning = false;
		StopGlide();
		m_move->SetEnabled(true);
		m_animationMesh->SwitchModelSet("default");
		m_gravDirection = m_world.Down();
		UseMagnet(true);
		m_vel = Vector::Zero;
		m_gravVel = Vector::Zero;
		m_velTotal = Vector::Zero;
		return;
	}

	m_pos = Vector3::Lerp(m_respawnStartPos, m_respawnEndPos, m_elapsedRespawnTime / m_totalRespawnTime);
	Quaternion quatRot = Quaternion::Slerp(m_respawnStartRot, m_respawnEndRot, m_elapsedRespawnTime / m_totalRespawnTime);
	m_rot = Matrix::CreateFromQuaternion(quatRot);
	UpdateWorld();
}

void Player::SetWaypoint(int _waypoint)
{
	m_move->SetWaypoint(_waypoint);
	m_waypoint = _waypoint;
}