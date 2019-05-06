#pragma once
#include "Item.h"
#include "ItemGrowthData.h"

class Player;

class LightningCloud : public Item
{
public:
	LightningCloud();
	void initCloudData();
	~LightningCloud() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

	void SetElapsedStrikeTime(float _time) { m_strikeTimeElapsed = _time; };
	float GetElapsedStrikeTime() { return m_strikeTimeElapsed; };

private:
	float m_strikeTimeElapsed = 0;
	float m_strikeDuration = 0;
	float m_slowAmount = 0;
	float m_maxSpeedMutli = 0;
	float m_playerSpinRev = 0;
	float m_playerSpinDuration = 0;

	bool m_striked = false;
	bool m_slowed = false;

	ItemGrowthData m_growthData;
};
