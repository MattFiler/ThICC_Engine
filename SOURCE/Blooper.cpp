#include "pch.h"
#include "Blooper.h"
#include "Player.h"


Blooper::Blooper(std::vector<Player*> _players) : m_players(_players)
{
	m_shouldDespawn = false;
}

void Blooper::Tick()
{
	switch (m_blooperState)
	{
		case UP:
		{
			std::string gliding = m_player->IsGliding() ? " Gliding" : "";
			SDKMeshGO3D* model = m_usingPlayerData.m_player->GetAnimController()->GetModelFromSet("Blooper" + gliding, "Blooper")->GetModel();

			model->SetPos(Vector3::Lerp(m_usingPlayerData.m_startPos, m_usingPlayerData.m_endPos, m_usingPlayerData.m_lerpPercent));
			m_usingPlayerData.m_lerpPercent += m_lerpSpeed * Locator::getGSD()->m_dt;

			if (m_usingPlayerData.m_lerpPercent >= 1)
			{
				m_player->GetAnimController()->SwitchModelSet(m_player->IsGliding() ? "gliding" : "defualt");
				m_blooperState = DOWN;
			}

			break;
		}
		case DOWN:
		{
			break;
		}
		case INK:
		{
			break;
		}
	}
}

void Blooper::Use(Player * player, bool _altUse)
{
	setItemInUse(player);

	m_usingPlayerData.m_player = m_player;
	m_usingPlayerData.m_startPos = m_player->GetAnimController()->GetModelFromSet("Blooper", "Blooper")->GetModel()->GetPos();
	m_usingPlayerData.m_endPos = Vector3(m_usingPlayerData.m_startPos.x, m_usingPlayerData.m_startPos.y + m_height, m_usingPlayerData.m_startPos.z);
	std::string gliding = m_player->IsGliding() ? " Gliding" : "";
	m_player->GetAnimController()->SwitchModelSet("Blooper" + gliding);

	FindPlayers();
}

void Blooper::FindPlayers()
{
	for (auto it = m_players.begin(); it != m_players.end();)
	{
		if ((*it)->GetRanking() >= m_player->GetRanking())
		{
			it = m_players.erase(it);
		}
		else
		{
			BlooperData data = BlooperData();
			data.m_player = (*it);
			data.m_endPos = (*it)->GetAnimController()->GetModelFromSet("Blooper", "Blooper")->GetModel()->GetPos();
			data.m_startPos = Vector3(data.m_endPos.x, data.m_endPos.y + m_height, data.m_endPos.z);
			m_blooperDatas.push_back(data);
			++it;
		}
	}

	m_players.clear();
}

