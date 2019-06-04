#pragma once
#include <queue>
#include <map>
#include "AnimationModel.h"
#include "GameFilepaths.h"

class AnimationController
{
public:
	AnimationController() = default;

	void Update(Matrix _parentWorld);
	void Render();

	void UpdateWorld(Matrix& _newWorld);

	void Jump(float _jumpHeight, float _duration);
	void Spin(int _revolutions, float _duration);
	void Flip(int _revolutions, float _duration);
	void Scale(Vector3 _newScale, float _duration);

	void Load();
	void Reset();

	void AddModel(std::string _name, std::string _filepath);
	void AddModel(std::string _name, SDKMeshGO3D* _model);
	AnimationModel* GetModelFromSet(std::string name, std::string _modelName);

	void AddModelSet(std::string _setName, std::vector<std::string> models);
	void SwitchModelSet(std::string _setName);
	std::string GetCurrentSet() { return m_currentSet; }

	void SetShouldRender(bool _shouldRender) { m_shouldRender = _shouldRender; };

	Vector3 GetScaleOffset() { return m_scaleOffset; };
	Vector3 GetPosOffset() { return m_posOffset; };
	void SetPosOffSet(Vector3 _offset) { m_posOffset = _offset; };
	void SetLockPosOffSet(bool _lock) { m_lockPosOffset = _lock; };

	void setLockSet(bool _lockSet) { m_lockSet = _lockSet; };
	bool getLockSet() { return m_lockSet; };

	void SetRotOffset(Vector3 _offset) { m_rotOffsetOverride = _offset; };

	bool FinishedScale() { return m_scaleTimeElapsed >= m_timeForScale; };
	void ResetScale();
	void ResetRotation();
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
	void UpdateScale();

	Matrix m_world;
	Matrix m_rot;
	Vector3 m_pos;
	Vector3 m_scale;
	Quaternion m_quatRot;

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

	Vector3 m_startScale = Vector3::One;
	Vector3 m_scaleOffset = Vector3::One;
	Vector3 m_targetScale = Vector3::One;
	float m_timeForScale = 1;
	float m_scaleTimeElapsed = 0;
	
	std::vector<std::unique_ptr<AnimationModel>> m_additionalModels;
	std::map <std::string, std::vector<AnimationModel*>> m_modelSet;

	std::string m_currentSet;

	bool m_shouldRender = true;
	bool m_lockSet = false;
	bool m_lockPosOffset = false;

	GameFilepaths m_filepath;

	Vector3 m_rotOffsetOverride = Vector3::Zero;

};