#pragma once
#include "Item.h"
#include "ItemCollisionData.h"

class Pow : public Item
{
public:
	Pow(std::vector<Player*> _players);
	~Pow();

	virtual void Tick() override;
	virtual void Use(Player* _player, bool _altUse) override;
	virtual void HitByPlayer(Player* _player) override {};
private:
	void FindPlayers();
	std::vector<Player*> m_players;
	ItemCollisionData m_collisionData;

	float m_warningTimeElapsed = 0;
	float m_maxWarningTime = 1;
	float m_heightShift = 0;
	int m_currentWarning = 0;
	int m_warningCount = 3;
};

