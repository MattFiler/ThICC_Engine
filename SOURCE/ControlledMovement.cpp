#include "pch.h"
#include "ControlledMovement.h"
#include "InputData.h"
#include "GameStateData.h"

ControlledMovement::ControlledMovement(PhysModel* _physModel, AnimationController* _animMesh) : m_physModel(_physModel), m_animMesh(_animMesh)
{
}

void ControlledMovement::Tick()
{
	GetControllerInput();
	ProcessInputFlags();
}

void ControlledMovement::GetControllerInput()
{

	//float rotSpeed = 0.05f;
	if (m_controlsActive)
	{
		m_isTurning = false;
		//GameController Movement
		if (m_keybind.keyHeld("drift", m_playerID))
		{
			m_isTurning = true;
			if (m_drifting == false)
			{
				m_startDrift = true;
				m_drifting = true;
				if (m_keybind.keyHeld("left", m_playerID))
				{
					m_driftingRight = false;
				}
				else if (m_keybind.keyHeld("right", m_playerID))
				{
					m_driftingRight = true;
				}
				else
				{
					m_isTurning = false;
					m_drifting = false;
				}
			}
		}
		else
		{
			if (m_drifting)
			{
				m_endDrift = true;
			}
		}

		if (m_keybind.keyHeld("accelerate", m_playerID))
		{
			m_acceleration = 1;
		}
		else if (m_keybind.keyHeld("decelerate", m_playerID))
		{
			m_acceleration = -0.5f;
		}
		else
		{
			m_acceleration = 0;
		}

		if (m_keybind.keyHeld("left", m_playerID))
		{
			m_left = true;
			m_right = false;
			m_isTurning = true;
		}
		else if (m_keybind.keyHeld("right", m_playerID))
		{
			m_right = true;
			m_left = false;
			m_isTurning = true;
		}
		else
		{
			if (!m_drifting)
			{
				m_isTurning = false;
			}
			m_right = false;
			m_left = false;
		}
	}
}


void ControlledMovement::ProcessInputFlags()
{
	if (!m_enabled)
	{
		m_animMesh->Update(m_physModel->GetWorld(), m_targetAnimRotOffset);
		return;
	}

	Vector3 forwardMove = m_moveSpeed * m_physModel->GetWorld().Forward();
	Vector3 rightMove = m_turnSpeed * m_physModel->GetWorld().Right();
	Vector3 forwardComponent = Vector3::Zero;
	Vector3 turnComponent = Vector3::Zero;
	float driftMultiplier = 1;

	Matrix rotMove = Matrix::CreateRotationY(m_physModel->GetYaw());
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	rightMove = Vector3::Transform(rightMove, rotMove);

	forwardComponent += forwardMove * m_acceleration;

	if (m_startDrift)
	{
		m_animMesh->Jump(0.5f, 0.25f);
		m_startDrift = false;
	}
	else if (m_endDrift)
	{
		EndDrift();
	}

	if (m_drifting)
	{
		if (m_driftingRight)
		{
			m_targetAnimRotOffset = (m_physModel->GetWorld().Forward() * 2) + m_physModel->GetWorld().Right();
		}
		else
		{
			m_targetAnimRotOffset = (m_physModel->GetWorld().Forward() * 2) + m_physModel->GetWorld().Left();
		}
	}

	if (m_left)
	{
		m_targetAnimRotOffset = (m_physModel->GetWorld().Forward() * 2.5f) + m_physModel->GetWorld().Left();
		if (m_drifting)
		{
			if (m_driftingRight)
			{
				m_targetAnimRotOffset = (m_physModel->GetWorld().Forward() * 3) + m_physModel->GetWorld().Right();
				driftMultiplier = 0.5f;
			}
			else
			{
				m_targetAnimRotOffset = (m_physModel->GetWorld().Forward()) + m_physModel->GetWorld().Left();
				driftMultiplier = 2;
			}
		}
		else
		{
			turnComponent -= rightMove;
		}
	}
	else if (m_right)
	{
		m_targetAnimRotOffset = (m_physModel->GetWorld().Forward() * 2.5f) + m_physModel->GetWorld().Right();
		if (m_drifting)
		{
			if (m_driftingRight)
			{
				m_targetAnimRotOffset = (m_physModel->GetWorld().Forward()) + m_physModel->GetWorld().Right();
				driftMultiplier = 2;
			}
			else
			{
				m_targetAnimRotOffset = (m_physModel->GetWorld().Forward() * 3) + m_physModel->GetWorld().Left();
				driftMultiplier = 0.5f;
			}
		}
		else
		{
			turnComponent += rightMove;
		}
	}
	else if (!m_drifting)
	{
		m_targetAnimRotOffset = m_physModel->GetWorld().Forward();
	}

	m_physModel->setAcceleration(forwardComponent);

	if (m_isTurning)
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
			//turnComponent *= driftMultiplier;
			turnComponent *= 1 + ((m_timeTurning / m_timeForMaxDrift) * driftMultiplier);
			//turnComponent *= 1 + (m_timeTurning / 1.3f);
		}
		else
		{
			if (m_timeTurning > m_timeForMaxTurn)
			{
				m_timeTurning = m_timeForMaxTurn;
			}
			m_timeTurning += Locator::getGSD()->m_dt;
			//turnComponent *= 1 + ((m_timeTurning / m_timeForMaxTurn)  * m_maxTurnRateMutliplier);
			//turnComponent *= 1 + (m_timeTurning / 1.3f);
		}

		float accLength = m_physModel->getAcceleration().Length();
		if (turnComponent.LengthSquared() > 0)
		{
			// Additing additional speed when turning else it doesn't feel right
			accLength += turnComponent.Length() / 4;
		}
		m_physModel->getAcceleration() += turnComponent;
		m_physModel->getAcceleration().Normalize();
		m_physModel->getAcceleration() *= accLength;

	}
	else
	{
		EndDrift();
	}

	m_animMesh->Update(m_physModel->GetWorld(), m_targetAnimRotOffset);
}

void ControlledMovement::EndDrift()
{
	Vector3 test = m_physModel->GetWorld().Forward();
	if (m_timeTurning > m_timeForMaxDrift / 3)
	{
		float multiplier = 1.0f + ((m_timeTurning / m_timeForMaxDrift)*m_driftBoostMultiplier);
		multiplier *= m_physModel->getVelocity().Length();
		Vector3 direction = m_physModel->GetWorld().Forward();
		m_physModel->setVelocity(direction * multiplier);
	}
	m_timeTurning = 0;
	m_endDrift = false;
	m_drifting = false;
}

void ControlledMovement::TurnLeft()
{ 
	m_left = true;
	m_right = false;
	m_isTurning = true;
}
void ControlledMovement::TurnRight()
{ 
	m_left = false;
	m_right = true;
	m_isTurning = true;
};

void ControlledMovement::DontTurn()
{
	m_left = false;
	m_right = false;
	m_isTurning = false;
}

void ControlledMovement::Drift(bool _flag)
{
	if (!_flag)
	{
		m_endDrift = true;
		m_drifting = false;
	}
	else if (!m_drifting)
	{
		m_drifting = true;
		m_startDrift = true;
		if (m_left)
		{
			m_driftingRight = false;
		}
		else
		{
			m_driftingRight = true;
		}
	}
}