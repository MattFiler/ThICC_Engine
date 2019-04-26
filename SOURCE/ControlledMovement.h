#pragma once
#include "PhysModel.h"
#include "AnimationController.h"
#include "KeybindManager.h"

class ControlledMovement
{
public:
	ControlledMovement() = default;
	ControlledMovement(PhysModel* _physModel, AnimationController* _animMesh);

	void Tick();
	void SetGamepadActive(bool _flag) { m_controlsActive = _flag;};
	void SetPlayerID(int _id) { m_playerID = _id;};

	// Setters so that the AI can move the physObject
	void setAcceleration(float _acc) { m_acceleration = _acc;};
	void TurnLeft();
	void TurnRight();
	void DontTurn();
	void Drift(bool _flag);

	bool IsTurningLeft() { return m_left; };
	bool IsTurningRight() { return m_right; };

	void SetWaypoint(int _waypoint) { m_waypoint = _waypoint; };
	int GetWaypoint() { return m_waypoint; };
	
	void SetMoveSpeed(float _newSpeed) { m_moveSpeed = _newSpeed; };
	void SetTurnSpeed(float _newTurn) { m_turnSpeed = _newTurn; };

	void SetEnabled(bool _enabled) { m_enabled = _enabled; };

private:
	KeybindManager m_keybind;

	void GetControllerInput();
	void ProcessInputFlags();

	void EndDrift();


	PhysModel* m_physModel = nullptr;
	AnimationController* m_animMesh = nullptr;
	Vector3 m_targetAnimRotOffset = Vector3::Zero;

	bool m_enabled = true;
	float m_acceleration = 0;
	bool m_isTurning = false;
	double m_timeTurning = 0;
	double m_timeForMaxTurn = 5;
	double m_timeForMaxDrift = 4;
	float m_driftBoostMultiplier = 0.3f;

	bool m_left = false;
	bool m_right = false;
	bool m_drifting = false;
	bool m_startDrift = false;
	bool m_endDrift = false;

	float m_moveSpeed = 50;
	float m_turnSpeed = 65;

	bool m_driftingRight = false;

	bool m_controlsActive = false;
	int m_playerID = 0;

	int m_waypoint = 0;

};