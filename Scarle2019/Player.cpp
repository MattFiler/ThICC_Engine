#include "pch.h"
#include "Player.h"
#include "GameStateData.h"
#include "ServiceLocator.h"
#include "InputData.h"
#include "ItemData.h"
#include <iostream>

extern void ExitGame();

Player::Player(string _filename, int _playerID, std::function<Item*(ItemType)> _createItemFunction) : TrackMagnet(_filename), CreateItem(_createItemFunction)
{
	m_RD = Locator::getRD();
	SetDrag(0.7);
	SetPhysicsOn(true);
	m_playerID = _playerID;
	m_textRanking = new Text2D(std::to_string(m_ranking));
	m_textRanking->SetScale(0.1f * Vector2::One);
	m_textLap = new Text2D(std::to_string(m_lap) + "/3");
	m_textCountdown = new Text2D("3");
	m_imgItem = Locator::getItemData()->GetItemSprite(PLACEHOLDER, m_playerID);
	m_textFinishOrder = new Text2D("0" + m_orderIndicator[0]);

	// Don't render this mesh, render a second one instead
	m_shouldRender = false;
	m_displayedMesh = std::make_unique<AnimationMesh>(_filename);

	m_move = ControlledMovement(this, m_displayedMesh.get());

	for (int i = 0; i < (int)m_posHistoryLength / m_posHistoryInterval; i++)
	{
		m_posHistory.push(m_world);
	}
}

Player::~Player()
{
	delete m_imgItem;
	m_imgItem = nullptr;
}


void Player::setActiveItem(ItemType _item) {
	if (inventory_item == _item) {
		active_item = _item;
		m_imgItem = Locator::getItemData()->GetItemSprite(PLACEHOLDER, m_playerID);
		m_imgItem->SetPos(m_itemPos);
		inventory_item = ItemType::NONE;
		std::cout << "PLAYER " << m_playerID << " HAS ACTIVATED ITEM: " << _item << std::endl; //debug
	}
	else
	{
		//We should never get here
		std::cout << "Player tried to use an item that they did not have. This should never be requested!" << std::endl;
	}
};

void Player::setItemInInventory(ItemType _item) {
	if (inventory_item == ItemType::NONE) {
		inventory_item = _item;
		m_imgItem = Locator::getItemData()->GetItemSprite(_item, m_playerID);
		m_imgItem->SetPos(m_itemPos);
		std::cout << "PLAYER " << m_playerID << " HAS ACQUIRED ITEM: " << _item << std::endl; //debug
	}
}

void Player::Render()
{
	m_displayedMesh->Render();
	SDKMeshGO3D::Render();
}


void Player::Tick()
{
	movement();

	RespawnLogic();
	

	// Debug code to save/load the players game state
	if (m_keymindManager.keyPressed("Debug Save Matrix"))
	{
		m_savedMatrix = m_world;
		m_savedVel = m_vel;
		m_savedGravVel = m_gravVel;
		m_savedGravDir = m_gravDirection;
	}
	else if (m_keymindManager.keyPressed("Debug Load Matrix"))
	{
		SetWorld(m_savedMatrix);
		m_vel = m_savedVel;
		m_gravVel = m_savedGravVel;
		m_velTotal = m_vel + m_savedGravVel;
		m_gravDirection = m_savedGravDir;
	}
	else if (m_keymindManager.keyPressed("Spawn Banana"))
	{
		SpawnItems(ItemType::GREEN_SHELL);
	}
	else if (m_keymindManager.keyHeld("Spawn Banana"))
	{
		TrailItems();
	}
	/*else
	{
		ReleaseItem();
	}*/

	//Debug output player location - useful for setting up spawns
	if (m_keymindManager.keyPressed("Debug Print Player Location")) {
		std::cout << "PLAYER POSITION: (" << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << ")" << std::endl;
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

void Player::TrailItems()
{
	if (!m_trailingItems.empty())
	{
		for (int i = 0; i < m_trailingItems.size(); i++)
		{
			Vector3 backward_pos = i > 0 ? m_trailingItems[i - 1]->GetMesh()->GetWorld().Backward() : m_world.Backward();

			m_trailingItems[i]->GetMesh()->SetWorld(m_world);
			m_trailingItems[i]->GetMesh()->AddPos(backward_pos * 2.2 + (backward_pos * 1.5 * i));
			m_trailingItems[i]->GetMesh()->UpdateWorld();
		}
		
	}
}

void Player::SpawnItems(ItemType type)
{
	setActiveItem(type);
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
		}

		case BANANA_3X:
		{
			for (int i = 0; i < 3; i++)
			{
				SpawnItems(BANANA);
				TrailItems();
			}

			for (Item*& banana : m_trailingItems)
			{
				banana->addImmuneItems(m_trailingItems);
			}

			m_tripleItem = true;
		}
		default:
			break;
	}
}

void Player::ReleaseItem()
{
	if (!m_trailingItems.empty())
	{
		std::cout << "Release item" << std::endl;
		if (m_trailingItems.size() > 1)
		{
			m_trailingItems[m_trailingItems.size() - 1]->Use(this, Locator::getGSD()->m_gamePadState[m_playerID].IsLeftShoulderPressed());
			m_trailingItems.pop_back();
		}
		else
		{
			m_trailingItems[0]->Use(this, Locator::getGSD()->m_gamePadState[m_playerID].IsLeftShoulderPressed());
			m_trailingItems.erase(m_trailingItems.begin());
		}

		if (m_trailingItems.empty())
		{
			m_tripleItem = false;
			active_item = NONE;
		}
	}
}

void Player::setGamePad(bool _state)
{
	m_move.SetGamepadActive(_state);
	m_move.SetPlayerID(m_playerID);
}

void Player::movement()
{
	m_move.Tick();

	Locator::getID()->m_gamePad->SetVibration(m_playerID, Locator::getGSD()->m_gamePadState[m_playerID].triggers.right * 0.1, Locator::getGSD()->m_gamePadState[m_playerID].triggers.right * 0.1);

	// Debug code to save/load the players game state
	if (m_keymindManager.keyPressed("Debug Save Matrix"))
	{
		m_savedMatrix = m_world;
		m_savedVel = m_vel;
		m_savedGravVel = m_gravVel;
	}
	else if (m_keymindManager.keyPressed("Debug Load Matrix"))
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
	if (m_keymindManager.keyPressed("Debug Print Player Location")) {
		std::cout << "PLAYER POSITION: (" << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << ")" << std::endl;
	}

}

void Player::RespawnLogic()
{
	m_posHistoryTimer += Locator::getGSD()->m_dt;
	if (m_onTrack)
	{
		if (m_posHistoryTimer >= m_posHistoryInterval)
		{
			m_posHistoryTimer -= m_posHistoryInterval;
			m_posHistory.push(m_world);
			m_posHistory.pop();
		}
	}
	else
	{
		if (m_posHistoryTimer >= m_respawnDelay)
		{
			m_posHistoryTimer = 0;
			SetWorld(m_posHistory.front());
			m_vel = Vector::Zero;
			m_gravVel = Vector::Zero;
			m_velTotal = Vector::Zero;
		}
	}
}