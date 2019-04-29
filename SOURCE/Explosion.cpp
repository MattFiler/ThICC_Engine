#include "pch.h"
#include "Explosion.h"
#include "GameStateData.h"
#include <iostream>
#include <fstream>

Explosion::Explosion(ItemType _ownerType) : TrackMagnet("bomb_explosion")
{
	/*InitExplosionData(_ownerType);

	m_growthData.m_shrinkScale = m_scale;
	m_growthData.m_growScale = m_growthData.m_shrinkScale * m_growthData.m_scaleMulti;
	Load();*/
}

void Explosion::InitExplosionData(ItemType _ownerType)
{
	/*std::string ownerName = "";
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
	m_growthData.m_scaleMulti = (float)data[ownerName]["info"]["explosion"]["size_multiplier"];
	m_growthData.m_growthSpeed = (float)data[ownerName]["info"]["explosion"]["growth_speed"];

	m_collisionData.m_playerVelMulti = (float)data[ownerName]["info"]["explosion"]["player_collision"]["velocity_multiplier"];
	m_collisionData.m_jumpHeight = (float)data[ownerName]["info"]["explosion"]["player_collision"]["jump"]["height"];
	m_collisionData.m_jumpDuration = (float)data[ownerName]["info"]["explosion"]["player_collision"]["jump"]["duration"];
	m_collisionData.m_flipRev = (float)data[ownerName]["info"]["explosion"]["player_collision"]["flip"]["revolutions"];
	m_collisionData.m_flipDuration = (float)data[ownerName]["info"]["explosion"]["player_collision"]["flip"]["duration"];
	m_collisionData.m_spinRev = (float)data[ownerName]["info"]["explosion"]["player_collision"]["spin"]["revolutions"];
	m_collisionData.m_spinDuration = (float)data[ownerName]["info"]["explosion"]["player_collision"]["spin"]["duration"];
	m_collisionData.m_vertPosOffset = (float)data[ownerName]["info"]["explosion"]["player_collision"]["vertical_pos_offset"];*/
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
	//TrackMagnet::Tick();

	//if (m_explode)
	//{
	//	m_scale = Vector3::Lerp(m_growthData.m_shrinkScale, m_growthData.m_growScale, m_growthData.m_scalePercent);
	//	UpdateWorld();
	//	m_growthData.m_scalePercent += m_growthData.m_growthSpeed * Locator::getGSD()->m_dt;
	//	if (m_growthData.m_scalePercent >= 1)
	//	{
	//		m_growthData.m_scalePercent = 1;
	//		m_shouldDestroy = true;
	//	}
	//}
}



