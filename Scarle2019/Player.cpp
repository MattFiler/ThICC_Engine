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

	m_targetAnimRotOffset = m_world.Forward();

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

	Animations();
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
	m_controlsActive = _state;
}

void Player::movement()
{
	bool isTurning = false;
	//FORWARD BACK & STRAFE CONTROL HERE
	Vector3 forwardMove = 25.0f * m_world.Forward();
	Vector3 rightMove = 12.5f * m_world.Right();
	Vector3 forwardComponent = Vector::Zero;
	Vector3 turnComponent = Vector::Zero;
	float driftMultiplier = 1;

	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	rightMove = Vector3::Transform(rightMove, rotMove);
	//float rotSpeed = 0.05f;

	if (m_playerID == 0)
	{
		if (m_keymindManager.keyHeld("Forward"))
		{
			//m_acc += forwardMove;
			forwardComponent += forwardMove;
		}
		if (m_keymindManager.keyHeld("Backwards"))
		{
			//m_acc -= forwardMove / 2;
			forwardComponent -= forwardMove / 2;
		}
		if (m_keymindManager.keyHeld("Left"))
		{
			m_acc -= rightMove;
			isTurning = true;
		}
		if (m_keymindManager.keyHeld("Right"))
		{
			m_acc += rightMove;
			isTurning = true;
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
				if (Locator::getGSD()->m_gamePadState[m_playerID].IsRightShoulderPressed())
				{
					isTurning = true;
					if (m_drifting == false)
					{
						m_displayedMesh->Jump(0.5f, 0.25f);
						Vector addVel = Vector::Zero;
						m_drifting = true;
						if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickLeft())
						{
							m_driftingRight = false;
						}
						else if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickRight())
						{
							m_driftingRight = true;
						}
						else
						{
							isTurning = false;
							m_drifting = false;
						}
						m_vel += addVel;
					}
					if (m_driftingRight)
					{
						m_targetAnimRotOffset = (m_world.Forward() * 2) + m_world.Right();
					}
					else
					{
						m_targetAnimRotOffset = (m_world.Forward() * 2) + m_world.Left();
					}
				}
				else
				{
					if (m_drifting)
					{
						EndDrift();
					}
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsRightTriggerPressed())
				{
					//m_acc += forwardMove * Locator::getGSD()->m_gamePadState[m_playerID].triggers.right;
					forwardComponent += forwardMove * Locator::getGSD()->m_gamePadState[m_playerID].triggers.right;
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftTriggerPressed())
				{
					//m_acc -= forwardMove / 2; //* _GSD->m_gamePadState->triggers.left;
					forwardComponent -= forwardMove / 2;
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickLeft())
				{
					m_targetAnimRotOffset = (m_world.Forward() * 2.5f) + m_world.Left();
					if (m_drifting)
					{
						if (m_driftingRight)
						{
							m_targetAnimRotOffset = (m_world.Forward() * 3) + m_world.Right();
							driftMultiplier = 0.1f;
						}
						else
						{
							m_targetAnimRotOffset = (m_world.Forward()) + m_world.Left();
							driftMultiplier = 1.5f;
						}
					}
					else
					{
						//m_acc -= rightMove;
						turnComponent -= rightMove;
					}
					isTurning = true;
				}
				else if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickRight())
				{
					m_targetAnimRotOffset = (m_world.Forward() * 2.5f) + m_world.Right();
					if (m_drifting)
					{
						if (m_driftingRight)
						{
							m_targetAnimRotOffset = (m_world.Forward()) + m_world.Right();
							driftMultiplier = 2;
						}
						else
						{
							m_targetAnimRotOffset = (m_world.Forward() * 3) + m_world.Left();
							driftMultiplier = 0.1f;
						}
					}
					else
					{
						turnComponent += rightMove;
					}
					isTurning = true;
				}
				else
				{
					if (!m_drifting)
					{
						m_targetAnimRotOffset = m_world.Forward();
					}
				}


				m_acc = forwardComponent;

				if (isTurning)
				{
					if (m_drifting)
					{
						if (m_timeTurning > m_timeForMaxDrift)
						{
							m_timeTurning = m_timeForMaxDrift;
						}
						if (m_driftingRight)
						{
							turnComponent = rightMove;
						}
						else
						{
							turnComponent = -rightMove;
						}

						m_timeTurning += Locator::getGSD()->m_dt * driftMultiplier;
						turnComponent *= 1 + ((m_timeTurning / m_timeForMaxDrift)  * m_maxDriftTurnMutliplier);
						turnComponent *= 1 + (m_timeTurning / 1.3f);
					}
					else
					{
						if (m_timeTurning > m_timeForMaxTurn)
						{
							m_timeTurning = m_timeForMaxTurn;
						}
						m_timeTurning += Locator::getGSD()->m_dt;
						turnComponent *= 1 + ((m_timeTurning / m_timeForMaxTurn)  * m_maxTurnRateMutliplier);
						turnComponent *= 1 + (m_timeTurning / 1.3f);
					}

					float accLength = m_acc.Length();
					if (turnComponent.LengthSquared() > 0)
					{
						accLength += turnComponent.Length() / 4;
					}
					m_acc += turnComponent;
					m_acc.Normalize();
					m_acc *= accLength;

				}
				else
				{
					EndDrift();
				}

				if (m_tripleItem)
				{
					TrailItems();

					if (Locator::getGSD()->m_gamePadState[m_playerID].IsAPressed())
					{
						if (m_trailingItems.empty() && inventory_item != NONE)
						{
							SpawnItems(inventory_item);
						}
						else if(!m_aPressed)
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
					if (Locator::getGSD()->m_gamePadState[m_playerID].IsAPressed())
					{
						if (m_trailingItems.empty() && inventory_item != NONE)
						{
							SpawnItems(inventory_item);
						}
						else
						{
							TrailItems();
						}
					}
					else
					{
						ReleaseItem();
					}
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

}

void Player::EndDrift()
{
	m_drifting = false;
	if (m_timeTurning > m_timeForMaxDrift / 3)
	{
		m_vel = m_world.Forward() * m_vel.Length();
	}
	m_timeTurning = 0;
}

void Player::Animations()
{
	m_displayedMesh->Update(m_world ,m_targetAnimRotOffset);
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