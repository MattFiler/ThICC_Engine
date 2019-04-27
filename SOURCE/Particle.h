#pragma once

#include "SDKMeshGO3D.h"

class Particle : public SDKMeshGO3D
{
public:
	Particle(std::string filename);
	~Particle();

	void Tick(Matrix world);

	void Render();

	void reset(float _lifetime, Vector3 _direction, Vector3* _start_pos);

	void reset(float _lifetime, Vector3 _direction, Vector3 _start_pos);

	float GetLifetime() { return lifetime; }

	void SetOffset(Vector3 _offset) { offset = _offset; }

	Vector3 GetOffset() { return offset; }

	void SetDirection(Vector3 _direction) { direction = _direction; }

	Vector3 GetDirection() { return direction; }

	bool IsDead() { return lifetime <= 0.0f; }

	bool IsUsed() { return used; }

private:

	float speed = 0.0f;
	float lifetime = 0.0f;
	Vector3 start_pos;
	Vector3 offset{0,0,0};
	Vector3 direction;
	bool used = false;
};