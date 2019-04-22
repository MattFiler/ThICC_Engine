#pragma once

#include "pch.h"
#include "Player.h"

class Explosion : public TrackMagnet
{
public:
	Explosion();
	~Explosion() = default;

	void HitByPlayer(Player* _player);
	virtual void Tick() override;

	void explode() { m_explode = true; };
private:
	bool m_explode = false;
	Vector3 m_startSize;
	Vector3 m_endSize;
	float m_percent = 0;
	float m_speed = 1.5;

};

