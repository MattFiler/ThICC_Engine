#pragma once
#include "PhysModel.h"
#include "Track.h"
#include "ControlledMovement.h"
#include <vector>

class MoveAI
{
public:
	MoveAI(PhysModel* _model, ControlledMovement* _move);

	void Update(Track* _track);
	void DebugRender();

	void UseDrift(bool _flag) { m_useDrift = _flag;};
private:
	bool FindRoute(Track* _track, Matrix& _world, Vector3& _pos, Vector3& _direction, int _iterations, bool _allowTurn);
	int FindWorld(Track* _track, const Matrix& _startWorld, Matrix& _endWorld, const Vector3& _startPos, Vector3& _endPos, Vector3 _direction, const int& _steps, const int& _iteration);
	int GetStepsForDirection(Track* _track, Vector3 _direction);

	PhysModel* m_model = nullptr;
	ControlledMovement* m_move = nullptr;

	float m_aiPathStep = 5;
	int m_maxPathIterations = 25;
	bool m_offTrack = false;

	bool m_useDrift = false;

	Vector3 m_waypointPos = Vector3::Zero;
	std::vector<Vector3> m_route;
	std::vector<SDKMeshGO3D*> m_debugCups;
	int m_minFrontSpace = 10; // The minimum number of m_aiPathStep iterations required to be in front of the object, after which the AI turns
	int m_minSideSpace = 1; // The minimum number of m_aiPathStep buffer space between the object and the side of the track
};