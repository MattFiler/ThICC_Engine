#include "pch.h"
#include "Player.h"
#include "GameStateData.h"
#include "ServiceLocator.h"
#include "InputData.h"
#include <iostream>
#include <iomanip>

extern void ExitGame();

Player::Player(string _filename, int _playerID, std::function<Item*(ItemType)> _createItemFunction) : TrackMagnet(_filename), CreateItem(_createItemFunction)
{
	m_RD = Locator::getRD();
	SetDrag(0.7);
	SetPhysicsOn(true);
	// SetPhysicsOn(false);
	m_playerID = _playerID;
	text_ranking = new Text2D(std::to_string(ranking));
	text_lap = new Text2D(std::to_string(lap) + "/3");
	countdown = new Text2D("0");
	m_totalLapTimeText = new Text2D("00:00:00");
	m_totalLapTimeText->SetScale( 0.1f * Vector2::One);
	item_img = new ImageGO2D("twist");
	item_img->SetScale(0.1f*Vector2::One);
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick()
{
	movement();

	//update timer
	if (lap < 3)
	{
		if (m_controlsActive)
		{
			m_seconds += Locator::getGSD()->m_dt;
			if (m_seconds > 60)
			{
				m_seconds = 0;
				m_minutes += 1;
			}
			m_totalLapTimeText->SetText("Time: " + std::to_string(m_hours) + ":" + std::to_string(m_minutes) + ":" + std::to_string((int)m_seconds));
			m_totalLapTimeText->SetScale(0.1f * Vector2::One);
		}
	}

	// Debug code to save/load the players game state
	if (m_keymindManager.keyPressed("Debug Save Matrix"))
	{
		// Debug code to save/load the players game state
		//FORWARD BACK & STRAFE CONTROL HERE
		Vector3 forwardMove = 30.0f * m_world.Forward();
		Vector3 rightMove = 60.0f * m_world.Right();
		Matrix rotMove = Matrix::CreateRotationY(m_yaw);
		forwardMove = Vector3::Transform(forwardMove, rotMove);
		rightMove = Vector3::Transform(rightMove, rotMove);
		//float rotSpeed = 0.05f;

	//Debug output player location - useful for setting up spawns
		if (m_keymindManager.keyPressed("Debug Print Player Location")) {
			std::cout << "PLAYER POSITION: (" << m_pos.x << ", " << m_pos.y << ", " << m_pos.z << ")" << std::endl;
		}

		//position->SetText(std::to_string(int(GetPos().x)) + ", " + std::to_string(int(GetPos().y)) + ", " + std::to_string(int(GetPos().z)), m_RD);
		position->SetText(std::to_string(ranking));
	}

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
		Banana* banana = static_cast<Banana*>(CreateItem(ItemType::BANANA));
		banana->SetWorld(m_world);
		banana->AddPos(m_world.Backward() * 2);
		banana->UpdateWorld();
	}
	else if (m_keymindManager.keyPressed("Spawn Green Shell"))
	{
		GreenShell* greenShell = static_cast<GreenShell*>(CreateItem(ItemType::GREEN_SHELL));
		greenShell->SetWorld(m_world);
		greenShell->AddPos(m_world.Forward() * 3);
		greenShell->UpdateWorld();
		greenShell->setVelocity(60 * m_world.Forward());
	}

	//apply my base behaviour
	PhysModel::Tick();

}

void Player::setGamePad(bool _state)
{
	m_controlsActive = true;
}

void Player::movement()
{
	//FORWARD BACK & STRAFE CONTROL HERE
	Vector3 forwardMove = 30.0f * m_world.Forward();
	Vector3 rightMove = 60.0f * m_world.Right();
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	rightMove = Vector3::Transform(rightMove, rotMove);

	if (m_controlsActive)
	{
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
					m_acc += forwardMove /* Locator::getGSD()->m_gamePadState[m_playerID].triggers.right*/;
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftTriggerPressed())
				{
					m_acc -= forwardMove; //* _GSD->m_gamePadState->triggers.left;
				}

				if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickLeft())
				{
					// *Locator::getGSD()->m_gamePadState[m_playerID].buttons.leftStick;
							//change orinetation of player

					if (m_vel.Length() < 1.0f)
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
					if (m_vel.Length() < 1.0)
					{
						m_acc += rightMove / 1000;
					}
					else
					{
						m_acc += rightMove;
					}
					
					 //* Locator::getGSD()->m_gamePadState[m_playerID].buttons.leftStick;
					//change orinetation of player
				}
			}
		}
		//m_pitch += rotSpeed * Locator::getGSD()->m_gamePadState[m_playerID].thumbSticks.rightY;
	}
	//Car rumble
	Locator::getID()->m_gamePad->SetVibration(m_playerID, Locator::getGSD()->m_gamePadState[m_playerID].triggers.right * 0.1, Locator::getGSD()->m_gamePadState[m_playerID].triggers.right * 0.1);
}