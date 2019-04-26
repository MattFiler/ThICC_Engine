#include "pch.h"
#include "Explosion.h"
#include "GameStateData.h"
#include <iostream>
#include <fstream>

Explosion::Explosion(ItemType _ownerType) : TrackMagnet("bomb_explosion")
{
	initExplosionData(_ownerType);

	m_startSize = m_scale;
	m_endSize = m_startSize * m_sizeMulti;
	Load();
}

void Explosion::initExplosionData(ItemType _ownerType)
{
	std::string ownerName = "";
	if (_ownerType == BOMB)
	{
		ownerName = "BOMB";
	}
	else if (_ownerType == BLUE_SHELL)
	{
		ownerName = "BLUE_SHELL";
	}
	else
	{
		throw std::runtime_error("ITEM OF THAT TYPE DOES NOT CREATE EXPLOSION");
	}

	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	json data;
	data << i;
	m_sizeMulti = (float)data[ownerName]["info"]["explosion"]["size_multiplier"];
	m_speed = (float)data[ownerName]["info"]["explosion"]["speed"];

	m_collisionData.m_playerVelMulti = (float)data[ownerName]["info"]["explosion"]["player_collision"]["velocity_multiplier"];
	m_collisionData.m_jumpHeight = (float)data[ownerName]["info"]["explosion"]["player_collision"]["jump"]["height"];
	m_collisionData.m_jumpDuration = (float)data[ownerName]["info"]["explosion"]["player_collision"]["jump"]["duration"];
	m_collisionData.m_flipRev = (float)data[ownerName]["info"]["explosion"]["player_collision"]["flip"]["revolutions"];
	m_collisionData.m_flipDuration = (float)data[ownerName]["info"]["explosion"]["player_collision"]["flip"]["duration"];
	m_collisionData.m_spinRev = (float)data[ownerName]["info"]["explosion"]["player_collision"]["spin"]["revolutions"];
	m_collisionData.m_spinDuration = (float)data[ownerName]["info"]["explosion"]["player_collision"]["spin"]["duration"];
	m_collisionData.m_vertPosOffset = (float)data[ownerName]["info"]["explosion"]["player_collision"]["vertical_pos_offset"];
}

void Explosion::HitByPlayer(Player * _player)
{
	_player->setVelocity(_player->getVelocity() * m_collisionData.m_playerVelMulti);
	_player->Jump(m_collisionData.m_jumpHeight, m_collisionData.m_jumpDuration);
	_player->Flip(m_collisionData.m_flipRev, m_collisionData.m_flipDuration);
	_player->Spin(m_collisionData.m_spinRev, m_collisionData.m_spinDuration);
	_player->AddPos(_player->GetWorld().Up() * m_collisionData.m_vertPosOffset);
	_player->UpdateWorld();
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



