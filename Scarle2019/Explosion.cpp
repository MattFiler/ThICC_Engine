#include "pch.h"
#include "Explosion.h"


Explosion::Explosion() : TrackMagnet("bomb_explosion"){}

void Explosion::HitByPlayer(Player * _player)
{
	_player->setVelocity(Vector3::Zero);
	//Flip the player
}


