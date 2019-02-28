#pragma once
#include "ItemUI.h"
#include "LapUI.h"
#include "PositionUI.h"

class PlayerHUD : public GameObject2D
{
public:
	PlayerHUD(RenderData* _renderData, int _iD);
	~PlayerHUD();

private :
	ItemUI *m_itemUI = nullptr;
	LapUI *m_lapUI = nullptr;
	PositionUI *m_positionUI = nullptr;

	int m_playerID = -1;

	RenderData *m_RD = nullptr;
};

