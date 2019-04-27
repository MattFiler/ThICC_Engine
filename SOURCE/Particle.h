#pragma once

#include "SDKMeshGO3D.h"

class Particle : public SDKMeshGO3D
{
public:
	Particle(std::string filename);
	~Particle();

	void Tick();

	void Render();

	void reset(float _lifetime, Vector3 _direction, Vector3 _start_pos);

	float GetLifetime() { return lifetime; }

	bool IsDead() { return lifetime <= 0.0f; }

	bool IsUsed() { return used; }

private:

	float speed = 0.0f;
	float lifetime = 0.0f;
	Vector3 direction;
	bool used = false;
};