#include "pch.h"
#include "Player.h"
#include "GameStateData.h"

Player::Player(RenderData* _RD, string _filename) : PhysModel(_RD, _filename)
{


	SetDrag(0.7);
	SetPhysicsOn(true);
}

Player::~Player()
{
	//tidy up anything I've created
}


void Player::Tick(GameStateData* _GSD)
{
	//FORWARD BACK & STRAFE CONTROL HERE
	Vector3 forwardMove = 40.0f * Vector3::Forward;
	Vector3 rightMove = 40.0f * Vector3::Right;
	Matrix rotMove = Matrix::CreateRotationY(m_yaw);
	forwardMove = Vector3::Transform(forwardMove, rotMove);
	rightMove = Vector3::Transform(rightMove, rotMove);
	if (_GSD->m_keyboardState.W)
	{
		m_acc += forwardMove;
	}
	if (_GSD->m_keyboardState.S)
	{
		m_acc -= forwardMove;
	}
	if (_GSD->m_keyboardState.A)
	{
		m_acc -= rightMove;
	}
	if (_GSD->m_keyboardState.D)
	{
		m_acc += rightMove;
	}

	//change orinetation of player
	float rotSpeed = 0.02f;
	m_yaw -= rotSpeed * _GSD->m_mouseState.x;

	//move player up and down
	if (_GSD->m_keyboardState.R)
	{
		m_acc.y += 40.0f;
	}

	if (_GSD->m_keyboardState.F)
	{
		m_acc.y -= 40.0f;
	}

	//limit motion of the player
	float length = m_pos.Length();
	float maxLength = 100.0f;
	if (length > maxLength)
	{
		m_pos.Normalize();
		m_pos *= maxLength;
		m_vel *= -0.9; //VERY simple bounce back
	}

	//apply my base behaviour
	PhysModel::Tick(_GSD);
}