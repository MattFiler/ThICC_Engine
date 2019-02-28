#include "pch.h"
#include "LapUI.h"


LapUI::LapUI(Vector2 _screenPos)
{
	m_lapText = new Text2D(std::to_string(1));
}

LapUI::~LapUI()
{

}
