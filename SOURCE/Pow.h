#pragma once
#include "Item.h"
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
};

