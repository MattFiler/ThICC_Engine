#pragma once
#include "Item.h"

class Player;

class LightningCloud : public Item
{
public:
	LightningCloud();
	~LightningCloud() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

	void SetElapsedStrikeTime(float _time) { m_strikeTimeElapsed = _time; };
	float GetElapsedStrikeTime() { return m_strikeTimeElapsed; };

private:

	float m_strikeTimeElapsed = 0;
	float m_strikeDuration = 10;
	bool m_striked = false;

	Vector3 m_startScale;
	Vector3 m_endScale;
	float m_scaleSpeed = 2;
	float m_scalePercent = 0;
	float m_shrinkTimeElapsed = 0;
	float m_shrinkDuration = 13;
	enum ScaleState { GROW, MAINTIAIN, SHRINK } m_scaleState = SHRINK;

	float m_slowAmount = 0.2f;
};
