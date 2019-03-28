#pragma once
#include "Player.h"
class Explosion : public TrackMagnet
{
public:
	Explosion();
	~Explosion() = default;

	void HitByPlayer(Player* _player);

};

