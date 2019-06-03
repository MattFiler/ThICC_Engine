#include "pch.h"
#include "BlueShell.h"


BlueShell::BlueShell(std::function<Explosion*(ItemType)> _CreateExplosionFunciton, std::vector<Player*> _players) : CreateExplosion(_CreateExplosionFunciton), m_players(_players)
, Item(BLUE_SHELL)
{
}

void BlueShell::Tick()
{

}

void BlueShell::HitByPlayer(Player * _player)
{
	if (_player == m_targetPlayer)
	{
		m_explosion = CreateExplosion(BLUE_SHELL);
		m_explosion->SetWorld(m_itemMesh->m_mesh->GetWorld());
		m_explosion->Explode();
		FlagForDestoy();
	}
}

void BlueShell::Use(Player * _player, bool _altUse)
{
	setItemInUse(_player);

	FindTargetPlayer();
}

void BlueShell::FindTargetPlayer()
{
	for (Player* player : m_players)
	{
		if (m_targetPlayer)
		{
			if (player->GetRanking() < m_targetPlayer->GetRanking())
			{
				m_targetPlayer = player;
			}
		}
		else
		{
			m_targetPlayer = player;
		}
	}

	m_players.clear();
}

