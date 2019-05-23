#include "pch.h"
#include "POW.h"
#include "Player.h"


Pow::Pow(std::vector<Player*> _players) : m_players(_players)
{
}


Pow::~Pow()
{
}

void Pow::Tick()
{
	if (m_itemUsed)
	{
		for (Player* player : m_players)
		{
			std::string set = m_player->GetAnimController()->GetCurrentSet();
			if (m_player->IsGliding() && set != "POW Gliding")
			{
				player->GetAnimController()->SwitchModelSet("POW Gliding");
			}
			else if (m_player->IsRespawning() && set != "POW Respawn")
			{
				player->GetAnimController()->SwitchModelSet("POW Respawn");
			}
			else if (!m_player->IsGliding() && !m_player->IsRespawning() && m_player->GetAnimController()->GetCurrentSet() != "POW")
			{
				player->GetAnimController()->SwitchModelSet("POW");
			}
		}
	}
}

void Pow::Use(Player * _player, bool _altUse)
{
	setItemInUse(_player);
	m_trailingPlayerImmunity = true;
	//FindPlayers();
}

void Pow::FindPlayers()
{
	for (int i = 0; i < m_players.size(); ++i)
	{
		if (m_players[i]->GetRanking() >= m_player->GetRanking())
		{
			m_players.erase(m_players.begin() + i);
		}
	}
}
