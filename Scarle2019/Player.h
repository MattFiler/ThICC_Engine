#pragma once
#include "PhysModel.h"

//=================================================================
//Base Player Class (i.e. a model GO3D the player controls)
//=================================================================

class Player : public PhysModel
{

public:
	Player(RenderData* _RD, string _filename);
	~Player();

	virtual void Tick(GameStateData* _GSD) override;

	void playerNum(int val);
protected:

private:

	int player_num = 0;
};