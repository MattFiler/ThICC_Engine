#pragma once

#include "PlayerHUD.h"

class UIManager : public GameObject2D
{
public:
	UIManager();
	~UIManager();

	//void Tick(GameStateData* _GSD);

	void AddPlayerHUD(int iD, Vector2 _screenPos);
	//void AddMiniMap();

private :
	std::vector<PlayerHUD*> m_playerHUD;
	//TO DO: add leaderboard, minimap, 
	
};

