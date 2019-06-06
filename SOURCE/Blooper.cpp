#include "pch.h"
#include "Blooper.h"
#include "Player.h"


Blooper::Blooper(std::vector<Player*> _players) : m_players(_players)
{
	m_shouldDespawn = false;
}

void Blooper::Tick()
{
	for (BlooperData& data : m_blooperDatas)
	{
		if (data.m_player->isInvincible() || data.m_player->IsRespawning())
		{
			for (int i = 0; i < 3; ++i)
			{
				data.m_finished[i] = true;
				m_usingPlayerData.m_player->GetAnimController()->SwitchModelSet(m_usingPlayerData.m_player->IsGliding() ? "gliding" : "default");
			}
		}
	}

	switch (m_blooperState)
	{
		case UP:
		{
			SwitchModelSet(m_usingPlayerData.m_player);

			std::string set = m_usingPlayerData.m_player->GetAnimController()->GetCurrentSet();
			AnimationModel* model = m_usingPlayerData.m_player->GetAnimController()->GetModelFromSet(set, "Blooper");

			model->SetPosOffset(Vector3::Lerp(m_usingPlayerData.m_startPos, m_usingPlayerData.m_endPos, m_usingPlayerData.m_lerpPercent));
			m_usingPlayerData.m_lerpPercent += m_lerpSpeed * Locator::getGSD()->m_dt;

			DebugText::print("Up: " + std::to_string(model->GetPosOffset().x) + " " + std::to_string(model->GetPosOffset().y) + " " + std::to_string(model->GetPosOffset().z));

			if (m_usingPlayerData.m_lerpPercent >= 1)
			{
				model->SetPosOffset(m_usingPlayerData.m_startPos);
				m_usingPlayerData.m_player->GetAnimController()->SwitchModelSet(m_usingPlayerData.m_player->IsGliding() ? "gliding" : "default");

				for (BlooperData& data : m_blooperDatas)
				{
					std::string gliding = data.m_player->IsGliding() ? " Gliding" : "";
					data.m_player->GetAnimController()->SwitchModelSet("Blooper" + gliding);
					data.m_startPos = data.m_player->GetWorld().Up() * m_height;
				}

				m_blooperState = DOWN;
			}

			break;
		}
		case DOWN:
		{
			for (BlooperData& data : m_blooperDatas)
			{
				if (data.m_finished[0])
				{
					continue;
				}

				SwitchModelSet(data.m_player);

				std::string set = data.m_player->GetAnimController()->GetCurrentSet();
				AnimationModel* model = data.m_player->GetAnimController()->GetModelFromSet(set, "Blooper");
				model->SetPosOffset(Vector3::Lerp(data.m_startPos, data.m_endPos, data.m_lerpPercent));
				DebugText::print("Down " + std::to_string(model->GetPosOffset().x) + " " + std::to_string(model->GetPosOffset().y) + " " + std::to_string(model->GetPosOffset().z));

				data.m_lerpPercent += m_lerpSpeed * Locator::getGSD()->m_dt;

				if (data.m_lerpPercent >= 1)
				{
					data.m_endPos = data.m_player->GetAnimController()->GetModelFromSet(set, "Blooper")->GetPosOffset();
					data.m_startPos = data.m_player->GetWorld().Up() * m_height;
					data.m_finished[0] = true;
					++m_stateCount;
				}
			}

			if (m_stateCount >= m_blooperDatas.size())
			{
				m_stateCount = 0;
				m_blooperState = INK;
			}

			break;
		}
		case INK:
		{
			for (BlooperData& data : m_blooperDatas)
			{
				if (data.m_finished[1])
				{
					continue;
				}

				data.m_inkTimeElapsed += Locator::getGSD()->m_dt;

				if (data.m_inkTimeElapsed >= data.m_inkDuration)
				{
					data.m_finished[1] = true;
					++m_stateCount;
				}
			}

			if (m_stateCount >= m_blooperDatas.size())
			{
				m_stateCount = 0;
				m_blooperState = LEAVE;
			}
			break;
		}
		case LEAVE:
		{
			for (BlooperData& data : m_blooperDatas)
			{
				if (data.m_finished[2])
				{
					continue;
				}

				SwitchModelSet(data.m_player);
				std::string set = data.m_player->GetAnimController()->GetCurrentSet();

				AnimationModel* model = data.m_player->GetAnimController()->GetModelFromSet(set, "Blooper");
				model->SetPosOffset(Vector3::Lerp(data.m_startPos, data.m_endPos, data.m_lerpPercent));

				data.m_lerpPercent -= m_lerpSpeed * 2 * Locator::getGSD()->m_dt;

				if (data.m_lerpPercent <= 0)
				{
					data.m_player->GetAnimController()->SwitchModelSet(data.m_player->IsGliding() ? "gliding" : "default");
					model->SetPosOffset(data.m_endPos);
					++m_finishedCount;
					data.m_finished[2] = true;
				}
			}

			if (m_finishedCount >= m_blooperDatas.size())
			{
				FlagForDestoy();
			}
			break;
		}
	}
}

void Blooper::SwitchModelSet(Player* _player)
{
	std::string set = _player->GetAnimController()->GetCurrentSet();
	if (_player->IsGliding() && set != "Blooper gliding")
	{
		_player->GetAnimController()->SwitchModelSet("Blooper gliding");
	}
	else if (set != "Blooper")
	{
		_player->GetAnimController()->SwitchModelSet("Blooper");
	}
}

void Blooper::Use(Player * player, bool _altUse)
{
	setItemInUse(player);

	m_usingPlayerData.m_player = m_player;
	m_usingPlayerData.m_startPos = m_player->GetAnimController()->GetModelFromSet("Blooper", "Blooper")->GetPosOffset();
	m_usingPlayerData.m_endPos = m_player->GetWorld().Up() * m_height;
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
			data.m_endPos = (*it)->GetAnimController()->GetModelFromSet("Blooper", "Blooper")->GetPosOffset();
			m_blooperDatas.push_back(data);
			++it;
		}
	}

	m_players.clear();
}

