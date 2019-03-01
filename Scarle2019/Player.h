#pragma once
#include "TrackMagnet.h"

//=================================================================
//Base Player Class (i.e. a model GO3D the player controls)
//=================================================================

class Player : public TrackMagnet
{

public:
	Player(RenderData* _RD, string _filename, int _playerID);
	~Player();

	virtual void Tick(GameStateData* _GSD) override;

protected:
	std::unique_ptr<DirectX::GamePad> m_gamePad;
	int m_playerID = 0;

};