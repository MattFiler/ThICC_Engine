#pragma once
#include "SDKMeshGO3D.h"
#include <queue>

class AnimationMesh : public SDKMeshGO3D
{
public:
	AnimationMesh(std::string _filepath);

	void Update(Matrix _parentWorld, Vector3 _rotOffsetOverride);
	void Render() override;

	void Jump(float _jumpHeight, float _duration);
	void Spin(int _revolutions, float _duration);
	void Flip(int _revolutions, float _duration);

	void AddModel(GameObject3D* _gameObject, SimpleMath::Vector3 _offset);

	enum direction
	{
		FORWARD,
		LEFT,
		BACKWARD,
		RIGHT,
		UP,
		DOWN
	};

private:
	void PopRotationQueue(Vector3 _targetRot);
	Vector3 GetRotationFromDirection(direction _prevDirection, Matrix _world);

	Vector3 m_posOffset = Vector3::Zero;
	Vector3 m_rotOffset = Vector3::Zero;

	float m_posAnimSpeed = 10;
	float m_rotAnimSpeed = 10;

	std::queue<Vector3> m_posAnimPoints;
	Vector3 m_prevPoint = Vector3::Zero;
	float m_timeBetweenPos = 0.5f;
	float m_posTimeElapsed = 0;

	std::queue<direction> m_rotAnimPoints;
	direction m_prevDirection = FORWARD;
	float m_timeBetweenRot = 0.5f;
	float m_rotTimeElapsed = 0;
	
	std::vector<std::pair<std::unique_ptr<GameObject3D>, SimpleMath::Vector3>> m_additionalModels;
};