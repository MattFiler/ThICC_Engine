#pragma once

#include "Particle.h"
#include "PhysModel.h"

class ParticlePool
{
public:
	ParticlePool(PhysModel* object);
	~ParticlePool();

	void init(int _particle_amount);

	void ActivateNextParticle();

	void Update();

	void Render();

private:

	const static int pool_size = 500;

	PhysModel* game_object = nullptr;

	Particle* particles[pool_size]{nullptr};
	Vector3 start_pos{ 0, 0, 0 };

	int particle_amount = 0;

	float x_upper_limit = 1.0f;
	float x_lower_limit = -1.0f;
	float y_upper_limit = 3.0f;
	float y_lower_limit = 1.0f;
	float z_upper_limit = 1.0f;
	float z_lower_limit = 0.0f;
	float emission_rate = 500.0;
	float timer = 0.0f;
	bool burst = false;

};