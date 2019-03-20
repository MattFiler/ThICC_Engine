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
	text_ranking = new Text2D(std::to_string(ranking));
	text_ranking->SetScale(0.1f * Vector2::One);
	text_lap = new Text2D(std::to_string(lap) + "/3");
	countdown = new Text2D("3");
	item_img = new ImageGO2D("ITEM_PLACEHOLDER");
}

Player::~Player()
{
	delete item_img;
	item_img = nullptr;
}


void Player::setActiveItem(ItemType _item) {
	if (inventory_item == _item) {
		active_item = _item;
		item_img->UpdateSprite("ITEM_PLACEHOLDER");
		inventory_item = ItemType::NONE;
		std::cout << "PLAYER " << m_playerID << " HAS ACTIVATED ITEM: " << _item << std::endl; //debug
	}
	else
	{
		//We should never get here - so if we do, throw a useful error.
		throw std::runtime_error("Player tried to use an item that they did not have. This should never be requested!");
	}
};

void Player::setItemInInventory(ItemType _item) {
	if (inventory_item == ItemType::NONE) {
		inventory_item = _item;
		item_img->UpdateSprite(Locator::getItemData()->GetItemSpriteName(_item));
		std::cout << "PLAYER " << m_playerID << " HAS ACQUIRED ITEM: " << _item << std::endl; //debug
	}
}


void Player::Tick()
{
	movement();

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
		Vector3 scale = Vector3::Zero;
	}
	else if (m_keymindManager.keyPressed("Spawn Banana"))
	{
		SpawnItem(ItemType::GREEN_SHELL);
	}
	else if (m_keymindManager.keyHeld("Spawn Banana"))
	{
		TrailItem();
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
	text_ranking->SetText(std::to_string(ranking));
	item_img->Tick();

	//apply my base behaviour
	//PhysModel::Tick();
}

void Player::TrailItem()
{
	m_isTrailing = true;
	m_trailingItem->GetMesh()->SetWorld(m_world);
	m_trailingItem->GetMesh()->AddPos(m_world.Backward() * 2.2);
	m_trailingItem->GetMesh()->UpdateWorld();
}

void Player::SpawnItem(ItemType type)
{
	setActiveItem(type);
	switch (type)
	{
		case BANANA:
		{
			Banana * banana = static_cast<Banana*>(CreateItem(ItemType::BANANA));
			m_trailingItem = banana;
			TrailItem();
			break;
		}

		case MUSHROOM:
		{
			Mushroom* mushroom = static_cast<Mushroom*>(CreateItem(ItemType::MUSHROOM));
			mushroom->Use(this);
			break;
		}

		case GREEN_SHELL:
		{
			GreenShell* shell = static_cast<GreenShell*>(CreateItem(ItemType::GREEN_SHELL));
			m_trailingItem = shell;
			TrailItem();
			break;
		}

		default:
			break;
	}
}

void Player::ReleaseItem()
{
	if (m_trailingItem)
	{
		m_trailingItem->Use(this);
		m_isTrailing = false;
		m_trailingItem = nullptr;
		active_item = NONE;
	}

	text_lap->SetText(std::to_string(lap) + "/3");
	text_ranking->SetText(std::to_string(waypoint));
}

void Player::setGamePad(bool _state)
{
	m_controlsActive = _state;
}

void Player::movement()
{
	//FORWARD BACK & STRAFE CONTROL HERE
	Vector3 forwardMove = 30.0f * m_world.Forward();
	Vector3 rightMove = 60.0f * m_world.Right();
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	rightMove = Vector3::Transform(rightMove, rotMove);
	//float rotSpeed = 0.05f;

	if (m_playerID == 0)
	{
		if (m_keymindManager.keyHeld("Forward"))
		{
			m_acc += forwardMove;
		}
		if (m_keymindManager.keyHeld("Backwards"))
		{
			m_acc -= forwardMove;
		}
		if (m_keymindManager.keyHeld("Left"))
		{
			m_acc -= rightMove;
		}
		if (m_keymindManager.keyHeld("Right"))
		{
			m_acc += rightMove;
		}
	}

	if (m_controlsActive)
	{
		//GameController Movement

		if (Locator::getGSD()->m_gamePadState[m_playerID].IsConnected())
		{
			if (Locator::getGSD()->m_gamePadState[m_playerID].IsViewPressed())
			{
				ExitGame();
			}
			else
			{
				if (Locator::getGSD()->m_gamePadState[m_playerID].IsRightTriggerPressed())
				{
					m_acc += forwardMove * Locator::getGSD()->m_gamePadState[m_playerID].triggers.right;
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftTriggerPressed())
				{
					m_acc -= forwardMove; //* _GSD->m_gamePadState->triggers.left;
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickLeft())
				{
					//m_acc -= rightMove;// *_GSD->m_gamePadState[m_playerID].buttons.leftStick;
					if (m_vel.Length() < 1.0)
					{
						m_acc -= rightMove / 1000;
					}
					else
					{
						m_acc -= rightMove;
					}
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickRight())
				{
					//m_acc += rightMove;// *_GSD->m_gamePadState[m_playerID].buttons.leftStick;
					if (m_vel.Length() < 1.0)
					{
						m_acc += rightMove / 1000;
					}
					else
					{
						m_acc += rightMove;
					}
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsAPressed() && (inventory_item != NONE || active_item != NONE))
				{
					if (!m_isTrailing)
					{
						SpawnItem(inventory_item);
					}
					else
					{
						TrailItem();
					}
				}
				else
				{
					ReleaseItem();
				}
			}
		}
	}

	////change orinetation of player
	//float rotSpeed = 0.001f;
	////m_yaw -= rotSpeed * _GSD->m_mouseState.x;
	////m_pitch -= rotSpeed * _GSD->m_mouseState.y;

	//m_yaw -= rotSpeed * Locator::getGSD()->m_gamePadState[m_playerID].thumbSticks.rightX;
	//m_pitch += rotSpeed * Locator::getGSD()->m_gamePadState[m_playerID].thumbSticks.rightY;
	//Car rumble
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

	//apply my base behaviour
	TrackMagnet::Tick();

}


