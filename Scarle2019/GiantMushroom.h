#pragma once
#include "Item.h"
class GiantMushroom :public Item
{
public:
	GiantMushroom() = default;
	~GiantMushroom() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

private:
	Vector3 m_startScale;
	Vector3 m_endScale;
	float m_scaleSpeed = 2;
	float m_scalePercent = 0;
	float m_growthTimeElapsed = 0;
	float m_maxGrowthTime = 12;
	enum ScaleState {GROW, MAINTIAIN, SHRINK} m_scaleState = GROW;


};

