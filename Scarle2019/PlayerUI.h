#pragma once

#include "GameFilepaths.h"
#include <json.hpp>
using json = nlohmann::json;

struct PlayerUIData 
{
	ImageGO2D m_itemBox;
	Vector2 m_itemBoxPos			= Vector2(0.0f,0.0f);

	ImageGO2D m_activeItem;
	Vector2 m_activeItemPos			= Vector2(0.0f, 0.0f);

	Text2D m_lapText;
	Vector2 m_lapTextPos			= Vector2(0.0f, 0.0f);

	Text2D m_racePosition;
	Vector2 m_racePositionPos		= Vector2(0.0f, 0.0f);
};

class PlayerUI
{
public:
	PlayerUI();
	~PlayerUI();

private:


};

