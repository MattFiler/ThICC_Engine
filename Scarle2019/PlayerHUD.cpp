#include "pch.h"
#include "PlayerHUD.h"

PlayerHUD::PlayerHUD(RenderData * _renderData, int _iD)
{
	// create a hud for specified player.

	//assign renderData for UI elements
	m_RD = _renderData;

	Vector2 test_pos(0,0);

	//m_itemUI = new ItemUI(test_pos);
}

PlayerHUD::~PlayerHUD()
{

}
