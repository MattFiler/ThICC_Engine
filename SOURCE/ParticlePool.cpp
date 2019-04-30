#include "pch.h"
#include "ParticlePool.h"
#include "GameStateData.h"
#include "random"
#include "DebugText.h"
#include "Player.h"



ParticlePool::ParticlePool(SDKMeshGO3D* object, PhysModel* _offsets) : game_object(object), offsets(_offsets)
{
	for (size_t i = 0; i < pool_size; i++)
	{
		particles[i] = new Particle("yo");
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

		particles[i]->SetScale(0.05f);
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
				std::random_device rd;
				std::mt19937 eng(rd());
				std::uniform_real_distribution<float> x_distr(x_lower_limit, x_upper_limit);
				std::uniform_real_distribution<float> y_distr(y_lower_limit, y_upper_limit);
				std::uniform_real_distribution<float> z_distr(z_lower_limit, z_upper_limit);

				Vector3 dir = Vector3{ x_distr(eng), y_distr(eng), z_distr(eng) };

				dir = Vector3::Transform(dir, game_object->GetOri());
				Vector3 pos{ 0, 0, 0 };

				if (game_object)
				{
					std::uniform_int_distribution<int> int_dist(0, 1);
					int ran_num = int_dist(eng);
					particles[i]->SetRightWheel(ran_num);
					if (ran_num == 0)
						dir.x *= -1;
					particles[i]->reset(0.2f, dir, game_object, offsets);
				}
				else
					particles[i]->reset(0.01f, dir, start_pos);


				timer = 1.0f;
				break;
			}
		}
	}
}

void ParticlePool::Update()
{
	//int am = 0;
	for (int i = 0; i < particle_amount; i++)
	{
		if (particles[i]->GetLifetime() <= 0.0f)
			continue;


		particles[i]->Tick(game_object ? game_object->GetOri() : Matrix::Identity);

		//particles[i]->SetOffset(particles[i]->GetOffset() + game_object->GetWorld().Down() * Locator::getGSD()->m_dt);

	}

	//DebugText::print(std::to_string(am));
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
