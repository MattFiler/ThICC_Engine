#include "pch.h"
#include "Particle.h"
#include "ServiceLocator.h"
#include "GameStateData.h"
#include "DebugText.h"
#include "Player.h"


Particle::Particle(std::string filename) : SDKMeshGO3D(filename)
{
	speed = 5.0f;
}

Particle::~Particle()
{

}

void Particle::Tick(const Matrix& world)
{

	//direction *= m_world.Down() * Locator::getGSD()->m_dt
	//direction = Vector3::Transform(direction, game_object->GetOri());
	offset += direction * (speed * Locator::getGSD()->m_dt);
	//m_pos = *start_pos + Vector3::Transform(offset, world);

	if (game_object)
	{
		if (dynamic_cast<Player*>(game_object))
		{
			m_rot = game_object->GetOri();
			m_pos = (left_wheel ? game_object->data.m_globalFrontBottomLeft : game_object->data.m_globalFrontBottomRight) + Vector3::Transform(offset, game_object->GetOri());
		}
	}
		//else
		//	m_pos = game_object->GetPos() + Vector3::Transform(offset, game_object->GetWorld());

		//SetOri(Vector3{ game_object->GetYaw(), game_object->GetPitch(), game_object->GetRoll() } * game_object->GetWorld().Forward());
/*	else
		m_pos = start_pos + Vector3::Transform(offset, Matrix::Identity);*/

	lifetime -= Locator::getGSD()->m_dt;
	used = true;

	SDKMeshGO3D::Tick();
}

void Particle::Render()
{

	SDKMeshGO3D::Render();
}

void Particle::reset(float _lifetime, Vector3 _direction, PhysModel * object)
{
	used = false;
	lifetime = _lifetime;
	offset = { 0,0,0 };
	direction = _direction;
	direction.Normalize();
	game_object = object;
}

void Particle::reset(float _lifetime, Vector3 _direction, Vector3 _start_pos)
{
	used = false;
	start_pos = _start_pos;
	lifetime = _lifetime;
	offset = { 0,0,0 };
	direction = _direction;
	direction.Normalize();
}
