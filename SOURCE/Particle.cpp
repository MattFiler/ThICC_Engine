#include "pch.h"
#include "Particle.h"
#include "ServiceLocator.h"
#include "GameStateData.h"


Particle::Particle(std::string filename) : SDKMeshGO3D(filename)
{
	speed = 1.0f;
}

Particle::~Particle()
{

}

void Particle::Tick()
{
	m_pos += (speed* Locator::getGSD()->m_dt) * direction;

	lifetime -= Locator::getGSD()->m_dt;

	SDKMeshGO3D::Tick();
}

void Particle::Render()
{

	SDKMeshGO3D::Render();
}

void Particle::reset(float _lifetime, Vector3 _direction, Vector3 _start_pos)
{
	m_pos = _start_pos;
	lifetime = _lifetime;
	direction = _direction;
	direction.Normalize();
}