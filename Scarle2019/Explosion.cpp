#include "pch.h"
#include "Explosion.h"
#include "GarbageCollector.h"


Explosion::Explosion() : TrackMagnet("bomb_explosion")
{
	m_startSize = m_scale;
	m_endSize = m_startSize * 10;
}

void Explosion::HitByPlayer(Player * _player)
{
	_player->setVelocity(Vector3::Zero);
	//Flip the player
}

void Explosion::Tick()
{
	TrackMagnet::Tick();

	if (m_explode)
	{
		m_scale = Vector3::Lerp(m_startSize, m_endSize, m_percent);
		UpdateWorld();
		m_percent += m_speed * Locator::getGSD()->m_dt;
		if (m_percent > 1)
		{
			m_percent = 1;
			Locator::getGarbageCollector()->DeletePointer(this);
		}
	}
}



