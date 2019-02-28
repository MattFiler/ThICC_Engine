#pragma once

#include "PlayerData.h"

/*
	Interface for the oberver.
*/
class Observer
{
public:
	Observer() = default;
	~Observer() = default;

	virtual void Update() = 0;
	virtual void Update(PlayerData* _playerData) = 0;
};

