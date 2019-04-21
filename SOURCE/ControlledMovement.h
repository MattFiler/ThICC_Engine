#pragma once
#include "PhysModel.h"
#include "AnimationMesh.h"

class ControlledMovement
{
public:
	ControlledMovement() = default;
	ControlledMovement(PhysModel* _physModel, AnimationMesh* _animMesh, AnimationMesh* _animMesh2 = nullptr);

	void Tick();
	void SetGamepadActive(bool _flag) { m_controlsActive = _flag;};
	void SetPlayerID(int _id) { m_playerID = _id;};

	// Setters so that the AI can move the physObject
	void setAcceleration(float _acc) { m_acceleration = _acc;};
	void TurnLeft(bool _flag);
	void TurnRight(bool _flag);
	void Drift(bool _flag);

	void SetWaypoint(int _waypoint) { m_waypoint = _waypoint; };
	int GetWaypoint() { return m_waypoint; };

private:
	void GetControllerInput();
	void ProcessInputFlags();

	void EndDrift();


	PhysModel* m_physModel = nullptr;
	AnimationMesh* m_animMesh = nullptr;
	AnimationMesh* m_animMesh2 = nullptr;
	Vector3 m_targetAnimRotOffset = Vector3::Zero;

	float m_acceleration = 0;
	bool m_isTurning = false;
	double m_timeTurning = 0;
	float m_maxTurnRateMutliplier = 2.3f;
	float m_maxDriftTurnMutliplier = 4.2f;
	double m_timeForMaxTurn = 4;
	double m_timeForMaxDrift = 8;

	bool m_left = false;
	bool m_right = false;
	bool m_drifting = false;
	bool m_startDrift = false;
	bool m_endDrift = false;

	bool m_driftingRight = false;
	float m_driftBoost = 300;

	bool m_controlsActive = false;
	int m_playerID = 0;

	int m_waypoint = 0;

};