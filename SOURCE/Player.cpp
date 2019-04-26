#include "pch.h"
#include "Player.h"
#include "GameStateData.h"
#include "ServiceLocator.h"
#include "InputData.h"
#include "ItemData.h"
#include "AIScheduler.h"
#include <iostream>

extern void ExitGame();

Player::Player(CharacterInfo _character, VehicleInfo _vehicle, int _playerID, std::function<Item*(ItemType)> _createItemFunction) : TrackMagnet(_character.model), CreateItem(_createItemFunction)
{
	m_RD = Locator::getRD();
	SetDrag(0.7);
	m_useGroundTypes = true;
	SetPhysicsOn(true);
	m_playerID = _playerID;
	m_textRanking = new Text2D(std::to_string(m_ranking));
	//m_textRanking->SetScale(0.1f * Vector2::One);
	m_textLap = new Text2D(std::to_string(m_lap) + "/3");
	m_textCountdown = new Text2D("3", true);
	m_imgItem = Locator::getItemData()->GetItemSprite(PLACEHOLDER, m_playerID);
	m_textFinishOrder = new Text2D("0" + m_orderIndicator[0]);

	// Don't render this mesh, render a second one instead
	m_shouldRender = false;

	m_move = std::make_unique<ControlledMovement>(this, m_animationMesh.get());

	for (int i = 0; i < m_posHistoryLength; i++)
	{
		m_posHistory.push(m_world);
	}
}

Player::~Player()
{
	delete m_imgItem;
	m_imgItem = nullptr;
}


void Player::Reload(CharacterInfo _character, VehicleInfo _vehicle) {

	std::ifstream i(m_filepath.generateConfigFilepath(_vehicle.model, m_filepath.MODEL));
	json m_model_config_vehicle;
	m_model_config_vehicle << i;

	m_animationMesh = std::make_unique<AnimationController>();
	m_animationMesh->AddModel("vehicle", _vehicle.model, Vector::Zero);
	SetScale(m_model_config_vehicle["modelscale"]);

	std::ifstream x(m_filepath.generateConfigFilepath(_character.model, m_filepath.MODEL));
	json m_model_config_character;
	m_model_config_character << x;

	SDKMeshGO3D* new_model = new SDKMeshGO3D(_character.model);
	new_model->SetScale(m_model_config_character["modelscale"]);
	m_animationMesh->AddModel("character", new_model, Vector3(0,0,0));
	m_animationMesh->AddModel("lakitu", "DEFAULT_ITEM", Vector3::Up * 4);
	m_animationMesh->AddModelSet("default", std::vector < std::string>{"vehicle", "character"});
	m_animationMesh->AddModelSet("respawn", std::vector < std::string>{"vehicle", "character", "lakitu"});
	m_animationMesh->SwitchModelSet("default");

	m_animationMesh->Load();

	ControlledMovement* old_movement = m_move.release();
	m_move = std::make_unique<ControlledMovement>(this, m_animationMesh.get());

	//Update TrackMagnet here too?
}


void Player::SetActiveItem(ItemType _item) {
	if (m_InventoryItem == _item) {
		active_item = _item;
		m_imgItem = Locator::getItemData()->GetItemSprite(PLACEHOLDER, m_playerID);
		m_imgItem->SetPos(m_itemPos);


		m_InventoryItem = ItemType::NONE;
		
		DebugText::print("PLAYER " + std::to_string(m_playerID) + " HAS ACTIVATED ITEM: " + std::to_string(_item));
	}
	else
	{
		//We should never get here
		DebugText::print("PLAYER TRIED TO USE AN ITEM THEY DON'T HAVE - THIS SHOULD NEVER BE REQUESTED!");
	}
};

void Player::SetItemInInventory(ItemType _item) {
	if (m_InventoryItem == ItemType::NONE) {
		m_InventoryItem = _item;
		m_imgItem = Locator::getItemData()->GetItemSprite(_item, m_playerID);
		m_imgItem->SetPos(m_itemPos);
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

void Player::Render()
{
	m_animationMesh->Render();
	TrackMagnet::Render();

	/*if (m_ai)
	{
		m_ai->DebugRender();
	}*/
}


void Player::Tick()
{
	movement();

	RespawnLogic();

	if (m_controlsActive)
	{
		CheckUseItem();
	}
	
	PositionFloatingItems();

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

	//Update UI
	if (!m_finished)
	{
		m_textRanking->SetText(std::to_string(m_ranking));
		m_textLap->SetText(std::to_string(m_lap) + "/3");
		m_imgItem->Tick();
	}

	//apply my base behaviour
	TrackMagnet::Tick();
}

void Player::PositionFloatingItems()
{
	for (int i = 0; i < m_floatingItems.size(); i++)
	{
		m_floatingItems[i]->GetMesh()->SetWorld(m_world);
		m_floatingItems[i]->GetMesh()->AddPos(m_world.Up() * 2);
		m_floatingItems[i]->GetMesh()->UpdateWorld();
	}
}

void Player::CheckUseItem()
{
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
	if (!m_trailingItems.empty())
	{
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

			if (m_trailingItems[i]->GetMesh())
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
					m_trailingItems[i]->GetMesh()->AddPos(backward_pos * 2.2 + (backward_pos * 1.5 * i));
					m_trailingItems[i]->GetMesh()->UpdateWorld();
				}
				//Spins around the player
				else
				{
					m_trailingItems[i]->GetMesh()->SetWorld(m_world);
					Vector3 m_dpos = Vector3{ 2, 0, 2 };
					m_trailingItems[i]->setSpinAngle(m_trailingItems[i]->getSpinAngle() + 350 * Locator::getGSD()->m_dt);
					m_trailingItems[i]->GetMesh()->AddPos(Vector3::Transform({ sin(m_trailingItems[i]->getSpinAngle() / 57.2958f) 
						* m_dpos.x, m_dpos.y, cos(m_trailingItems[i]->getSpinAngle() / 57.2958f) * m_dpos.z }, m_rot));
				}
			}

			m_trailingItems[i]->setTrailing(true);
		}	
	}
}

void Player::SpawnItems(ItemType type)
{
	//Triple mushrooms and Golden Mushroom still in inventory after use
	if (type != MUSHROOM_3X && type != MUSHROOM_UNLIMITED)
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
		m_trailingItems[m_trailingItems.size() - 1]->Use(this, m_keybind.keyHeld("trail items", m_playerID));
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

void Player::setGamePad(bool _state)
{
	m_move->SetGamepadActive(_state);
	m_move->SetPlayerID(m_playerID);
	m_controlsActive = _state;

	// TEST CODE //
	
	
	/*if (m_playerID == 0)
	{
		m_ai = std::make_unique<MoveAI>(this, m_move.get());
		m_ai->UseDrift(true);
		Locator::getAIScheduler()->AddAI(m_ai.get());
	}*/
	
	// TEST CODE //
}

void Player::movement()
{
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

void Player::RespawnLogic()
{
	if (m_respawning)
	{
		MovePlayerToTrack();
		return;
	}

	m_posHistoryTimer += Locator::getGSD()->m_dt;
	m_timeSinceRespawn += Locator::getGSD()->m_dt;
	if (m_onTrack && m_colType == CollisionType::ON_TRACK)
	{
		m_offTrackTimer = 0;
		m_offTerrainTimer = 0;
		if (m_posHistoryTimer >= m_posHistoryInterval)
		{
			m_posHistoryTimer -= m_posHistoryInterval;
			m_posHistory.push(m_world);
			m_posHistory.pop();
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

void Player::Respawn()
{
	m_move->SetEnabled(false);
	m_animationMesh->SwitchModelSet("respawn");
	m_respawning = true;
	m_respawnEnd = m_posHistory.front();
	m_respawnStart = m_world;

	// Decompose the matrix
	Vector3 pos;
	Vector3 scale;
	Quaternion rot;
	m_respawnEnd.Decompose(scale, rot, pos);
	// Add a bit of height to it to "drop" the player
	pos += m_respawnEnd.Up() * 3;

	// Rebuild the matrix
	Matrix mat_rot = Matrix::CreateFromQuaternion(rot);
	Matrix trans = Matrix::CreateTranslation(pos);
	Matrix mat_scale = Matrix::CreateScale(scale);
	m_respawnEnd = mat_scale * mat_rot * trans;

	// Find the distance to the respawn point
	float dist = Vector3::Distance(pos, m_pos);
	m_totalRespawnTime = dist / m_respawnSpeed;

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
		SetWorld(Matrix::Lerp(m_respawnStart, m_respawnEnd, 1));
		m_respawning = false;
		m_move->SetEnabled(true);
		m_animationMesh->SwitchModelSet("default");
		return;
	}

	SetWorld(Matrix::Lerp(m_respawnStart, m_respawnEnd, m_elapsedRespawnTime / m_totalRespawnTime));
}

void Player::SetWaypoint(int _waypoint)
{
	m_move->SetWaypoint(_waypoint);
	m_waypoint = _waypoint;
}