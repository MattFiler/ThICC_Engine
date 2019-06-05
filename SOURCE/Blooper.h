#pragma once
#include "Item.h"
class Blooper :public Item
{
public:
	Blooper(std::vector<Player*> _players);
	~Blooper() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* player) override {};
	void Use(Player* player, bool _altUse) override;

	void FindPlayers();

private:

	struct BlooperData
	{
		Player* m_player = nullptr;
		Vector3 m_startPos = Vector3::Zero;
		Vector3 m_endPos = Vector3::Zero;
		float m_lerpPercent = 0;
	};

	float m_lerpSpeed = 1;
	enum blooperState { UP, DOWN, INK } m_blooperState = UP;

	std::vector<BlooperData> m_blooperDatas;
	BlooperData m_usingPlayerData = BlooperData();
	std::vector<Player*> m_players;
	float m_height = 50;

};

