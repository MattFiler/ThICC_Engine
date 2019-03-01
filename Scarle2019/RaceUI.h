#pragma once

#include "Observer.h"
#include "PlayerData.h"
#include "GameObject2D.h"
#include <vector>

struct RenderData;

class RaceUI : public GameObject2D
{
public:
	RaceUI() = default;
	RaceUI(RenderData* _RD, Vector2 _uiPositions[4]);
	virtual ~RaceUI();

	void CentreOrigin();

	void Render(RenderData* _RD);
	void Reset() override {};

protected:
	Text2D*			m_lapCounter = nullptr;
	Text2D*			m_racePosition = nullptr;
	ImageGO2D*		m_currentItemSprite = nullptr;
	ImageGO2D*		m_itemSpriteBox = nullptr;
};

