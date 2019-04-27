#include "pch.h"
#include "Particle.h"
#include "ServiceLocator.h"
#include "GameStateData.h"
#include "DebugText.h"


Particle::Particle(std::string filename) : SDKMeshGO3D(filename)
{
	speed = 1.0f;
}

Particle::~Particle()
{

}

void Particle::Tick(Matrix world)
{
	//offset += direction * (speed* Locator::getGSD()->m_dt);
	////m_pos = *start_pos + Vector3::Transform(offset, world);
	//m_pos = start_pos + Vector3::Transform(offset, world);

	lifetime -= Locator::getGSD()->m_dt;
	used = true;

	SDKMeshGO3D::Tick();
}

void Particle::Render()
{

	SDKMeshGO3D::Render();
}

void Particle::reset(float _lifetime, Vector3 _direction, Vector3 *_start_pos)
{
	used = false;
//	start_pos = _start_pos;
	lifetime = _lifetime;
	offset = { 0,0,0 };
	direction = _direction;
	direction.Normalize();
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
