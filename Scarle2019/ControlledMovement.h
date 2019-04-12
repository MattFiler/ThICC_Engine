#pragma once
#include "PhysModel.h"

class ControlledMovement
{
public:
	ControlledMovement() = default;
	ControlledMovement(PhysModel* _physModel, AnimationMesh* _animMesh = nullptr);

	void Tick();
	void SetGamepadActive(bool _flag) { m_controlsActive = _flag;};
	void SetPlayerID(bool _id) { m_playerID = _id;};

	// Setters so that the AI can move the physObject
	void setAcceleration(float _acc) { m_acceleration = _acc;};
	void TurnLeft(bool _flag);
	void TurnRight(bool _flag);

private:
	void GetControllerInput();
	void ProcessInputFlags();

	void EndDrift();


	PhysModel* m_physModel = nullptr;
	AnimationMesh* m_animMesh = nullptr;
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

};