#include "pch.h"
#include "ParticlePool.h"
#include "GameStateData.h"
#include "random"



ParticlePool::ParticlePool(PhysModel* object) : game_object(object)
{
	for (size_t i = 0; i < pool_size; i++)
	{
		particles[i] = new Particle("KART_KNUCKLES");
	}
}

ParticlePool::~ParticlePool()
{

}

void ParticlePool::init(int _particle_amount)
{
	particle_amount = _particle_amount;
	for (size_t i = 0; i < pool_size; i++)
	{
		dynamic_cast<GameObject3D*>(particles[i])->Load();
	}
}

void ParticlePool::ActivateNextParticle()
{
	timer -= Locator::getGSD()->m_dt * emission_rate;
	if (timer <= 0.0f)
	{
		for (int i = 0; i < particle_amount; i++)
		{
			if (particles[i]->IsDead() && (burst ? !particles[i]->IsUsed() : true))
			{
				std::random_device rd; // obtain a random number from hardware
				std::mt19937 eng(rd()); // seed the generator
				std::uniform_real_distribution<float> x_distr(x_lower_limit, x_upper_limit);
				std::uniform_real_distribution<float> y_distr(y_lower_limit, y_upper_limit);
				std::uniform_real_distribution<float> z_distr(z_lower_limit, z_upper_limit);

				Vector3 dir = Vector3{ x_distr(eng), y_distr(eng), z_distr(eng) };

				dir = Vector3::Transform(dir, game_object->GetOri());
				Vector3 pos{ 0, 0, 0 };

				if (game_object)
				{
					std::uniform_int_distribution<int> int_dist(0, 1);

					if (int_dist(eng) == 0)
						pos = game_object->data.m_globalFrontBottomLeft;

					else if (int_dist(eng) == 1)
						pos = game_object->data.m_globalFrontBottomRight;

				}

				particles[i]->reset(0.1f, dir, game_object ? pos : start_pos);
				particles[i]->SetScale(0.2f);
				timer = 1.0f;
				break;
			}
		}
	}
}

void ParticlePool::Update()
{
	for (int i = 0; i < particle_amount; i++)
	{
		if (particles[i]->GetLifetime() <= 0.0f)
			continue;

		particles[i]->Tick();
		particles[i]->SetPos(particles[i]->GetPos() + game_object->GetWorld().Down() * Locator::getGSD()->m_dt);

	}
}

void ParticlePool::Render()
{
	for (int i = 0; i < particle_amount; i++)
	{
		if (particles[i]->GetLifetime() <= 0.0f)
			continue;

		particles[i]->Render();
	}
}
