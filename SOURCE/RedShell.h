#pragma once
#include "Item.h"
#include "MoveAI.h"
#include "ControlledMovement.h"
#include "ItemCollisionData.h"
#include "ItemAiData.h"

class RedShell : public Item
{
public:
	RedShell(std::vector<Player*> _players);
	void InitShellData();
	~RedShell() = default;

	virtual void Tick() override;
	void HitByPlayer(Player* _player) override;
	void Use(Player* _player, bool _altUse) override;

	std::unique_ptr<MoveAI>& GetAi() { return m_ai; };
private:
	void FindTargetPlayer();

	std::unique_ptr<MoveAI> m_ai = nullptr;
	std::unique_ptr<ControlledMovement> m_move = nullptr;
	
	float m_usePosOffset = 0;
	bool m_altUse = false;

	ItemCollisionData m_collisionData;
	ItemAiData m_aiData;

	std::vector<Player*> m_players;
	Player* m_targetPlayer = nullptr;
};

