#include "ControlledMovement.h"
#include "pch.h"

ControlledMovement::ControlledMovement(PhysModel* _physModel, AnimationMesh* _animMesh) : m_physModel(_physModel), m_animMesh(_animMesh)
{
}

void ControlledMovement::Tick()
{
	GetControllerInput();
	ProcessInputFlags();
}

void ControlledMovement::GetControllerInput()
{
	//FORWARD BACK & STRAFE CONTROL HERE

	//float rotSpeed = 0.05f;
	if (m_controlsActive && Locator::getGSD()->m_gamePadState[m_playerID].IsConnected())
	{
		m_isTurning = false;
		//GameController Movement
		if (Locator::getGSD()->m_gamePadState[m_playerID].IsRightShoulderPressed())
		{
			m_isTurning = true;
			if (m_drifting == false)
			{
				m_startDrift = true;
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
					m_isTurning = false;
					m_drifting = false;
				}
			}
		}
		else
		{
			if (m_drifting)
			{
				m_endDrift;
			}
		}

		if (Locator::getGSD()->m_gamePadState[m_playerID].IsRightTriggerPressed())
		{
			m_acceleration = 1;
		}
		else if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftTriggerPressed())
		{
			m_acceleration = -0.5f;
		}
		else
		{
			m_acceleration = 0;
		}

		if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickLeft())
		{
			m_left = true;
			m_right = false;
			m_isTurning = true;
		}
		else if (Locator::getGSD()->m_gamePadState[m_playerID].IsLeftThumbStickRight())
		{
			m_right = true;
			m_left = false;
			m_isTurning = true;
		}
		else
		{
			m_right = false;
			m_left = false;
		}
	}
}


void ControlledMovement::ProcessInputFlags()
{
	Vector3 forwardMove = 25.0f * m_physModel->GetWorld().Forward();
	Vector3 rightMove = 12.5f * m_physModel->GetWorld().Right();
	Vector3 forwardComponent = Vector::Zero;
	Vector3 turnComponent = Vector::Zero;
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
				driftMultiplier = 0.1f;
			}
			else
			{
				m_targetAnimRotOffset = (m_physModel->GetWorld().Forward()) + m_physModel->GetWorld().Left();
				driftMultiplier = 1.5f;
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
				driftMultiplier = 0.1f;
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

		float accLength = m_physModel->getAcceleration().Length();
		if (turnComponent.LengthSquared() > 0)
		{
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
	if (m_timeTurning > m_timeForMaxDrift / 3)
	{
		m_physModel->setVelocity(m_physModel->GetWorld().Forward() * m_physModel->getVelocity().Length());
	}
	m_timeTurning = 0;
	m_endDrift = false;
	m_drifting = false;
}

void ControlledMovement::TurnLeft(bool _flag)
{ 
	m_left = _flag;
	m_isTurning = _flag;
}
void ControlledMovement::TurnRight(bool _flag)
{ 
	m_right = _flag; 
	m_isTurning = _flag;
};
