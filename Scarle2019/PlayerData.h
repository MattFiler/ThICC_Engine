#pragma once

enum Items
{
	NONE = 0,
	BANANA = 1,
	GREENSHELL = 2,
};

struct PlayerData
{
	int 					m_lapNumber = 1; //current lap number.
	int 					m_racePosition = 1; //leaderboard position.

	float					m_lapTimer = 0.0F; //total lap timer.

	Items					m_currentItem = Items::NONE; //Sprite 
};

