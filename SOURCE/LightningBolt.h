#pragma once
#include "Item.h"
#include "ItemGrowthData.h"
class LightningBolt : public Item
{
public:
	LightningBolt(std::vector<Player*> _players);

	virtual void Tick() override;
	virtual void Use(Player* _player, bool _altUse) override;
	virtual void HitByPlayer(Player* _player) override {};
	
	void FindPlayers();

private:

	struct StrikeData
	{
		Player* m_player = nullptr;
		ItemGrowthData m_growthData;
		Matrix m_lightningRot = Matrix::Identity;
		bool m_finished = false;
	};

	std::vector<StrikeData> m_strikeDatas;
	std::vector<Player*> m_players;
	ItemGrowthData m_baseGrowthData;

	float m_maxSizeChangeDuration = 13;
	float m_playerMoveSpeed = 0;
	float m_playerTurnSpeed = 0;
	float m_playerSpinRev = 0;
	float m_playerSpinDuration = 0.5;
	int m_finishedCount = 0;
};

