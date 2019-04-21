#include "pch.h"
#include "Explosion.h"
#include "GameStateData.h"

Explosion::Explosion() : TrackMagnet("bomb_explosion")
{
	m_startSize = m_scale;
	m_endSize = m_startSize * 10;
}

void Explosion::HitByPlayer(Player * _player)
{
	_player->setVelocity(Vector3::Zero);
	_player->Jump(1.5f, 1);
	_player->Flip(1, 0.8f);
	_player->AddPos(_player->GetWorld().Up() * 4);
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
			m_shouldDestroy = true;
		}
	}
}



