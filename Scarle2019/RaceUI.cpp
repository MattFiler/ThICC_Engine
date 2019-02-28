#include "pch.h"
#include "RaceUI.h"

RaceUI::RaceUI(RenderData* _RD, Vector2 _uiPositions[4])
{
	//Initialize all the values for player race UI: Position, Lap and SpriteBox
	m_lapCounter =			new Text2D("Lap: " + std::to_string(1));
	m_racePosition =		new Text2D("Position: " + std::to_string(2));
	m_currentItemSprite =	new ImageGO2D(_RD, "banana");
	m_itemSpriteBox =		new ImageGO2D(_RD, "ItemBox");

	//set UI Positions
	m_lapCounter->SetPos(_uiPositions[0]);
	m_racePosition->SetPos(_uiPositions[1]);
	m_itemSpriteBox->SetPos(_uiPositions[2]);
	m_currentItemSprite->SetPos(_uiPositions[3]);
}

RaceUI::~RaceUI()
{

}

void RaceUI::CentreOrigin()
{

}

void RaceUI::Render(RenderData * _RD)
{
	m_lapCounter->Render(_RD);
	m_racePosition->Render(_RD);
	m_itemSpriteBox->Render(_RD);
	m_currentItemSprite->Render(_RD);
}
