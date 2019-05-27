#include "pch.h"
#include "POW.h"
#include "Player.h"
#include "GameStateData.h"


Pow::Pow(std::vector<Player*> _players) : m_players(_players)
{
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	m_itemData << i;
	m_maxWarningTime = (float)m_itemData["POW"]["info"]["warning_time"];
	m_warningCount = (float)m_itemData["POW"]["info"]["warning_count"];

	m_collisionData.m_playerVelMulti = (float)m_itemData["POW"]["info"]["player_collision"]["velocity_multiplier"];
	m_collisionData.m_jumpHeight = (float)m_itemData["POW"]["info"]["player_collision"]["jump"]["height"];
	m_collisionData.m_jumpDuration = (float)m_itemData["POW"]["info"]["player_collision"]["jump"]["duration"];
	m_collisionData.m_flipRev = (float)m_itemData["POW"]["info"]["player_collision"]["flip"]["revolutions"];
	m_collisionData.m_flipDuration = (float)m_itemData["POW"]["info"]["player_collision"]["flip"]["duration"];
	m_collisionData.m_spinRev = (float)m_itemData["POW"]["info"]["player_collision"]["spin"]["revolutions"];
	m_collisionData.m_spinDuration = (float)m_itemData["POW"]["info"]["player_collision"]["spin"]["duration"];
	m_collisionData.m_vertPosOffset = (float)m_itemData["POW"]["info"]["player_collision"]["vertical_pos_offset"];
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

		if (m_currentWarning < m_warningCount)
		{
			m_warningTimeElapsed += Locator::getGSD()->m_dt;
			if (m_warningTimeElapsed >= m_maxWarningTime)
			{
				++m_currentWarning;
				m_warningTimeElapsed = 0;

				for (Player*& player : m_players)
				{
					AnimationModel* model = player->GetAnimController()->GetModelFromSet(m_player->GetAnimController()->GetCurrentSet(), "POW");
					Vector3 scale = model->GetCurrentScale();
					model->SetCurrentScale(Vector3(scale.x, scale.y - m_heightShift, scale.z));

					player->SetCounteredPow(m_currentWarning == m_warningCount && player->GetKeyBindManager().keyHeld("item alternate use", player->GetPlayerId()));
				}
			}
		}
		else
		{
			for (Player*& player : m_players)
			{
				if (!player->CounteredPow())
				{
					player->setVelocity(player->getVelocity() * m_collisionData.m_playerVelMulti);
					player->Jump(m_collisionData.m_jumpHeight, m_collisionData.m_jumpDuration);
					player->Flip(m_collisionData.m_flipRev, m_collisionData.m_flipDuration);
					player->Spin(m_collisionData.m_spinRev, m_collisionData.m_spinDuration);
					player->AddPos(player->GetWorld().Up() * m_collisionData.m_vertPosOffset);
					player->UpdateWorld();
					player->DropItems();
				}

				player->GetAnimController()->GetModelFromSet(m_player->GetAnimController()->GetCurrentSet(), "POW")->ResetScale();

				std::string set = m_player->GetAnimController()->GetCurrentSet();
				if (set == "POW Gliding")
				{
					player->GetAnimController()->SwitchModelSet("Gliding");
				}
				else if (set == "POW Respawn")
				{
					player->GetAnimController()->SwitchModelSet("Respawn");
				}
				else if (m_player->GetAnimController()->GetCurrentSet() == "POW")
				{
					player->GetAnimController()->SwitchModelSet("default");
				}
			}
			FlagForDestoy();
		}
	}
}

void Pow::Use(Player * _player, bool _altUse)
{
	setItemInUse(_player);
	m_trailingPlayerImmunity = true;
	m_heightShift = m_player->GetAnimController()->GetModelFromSet("POW", "POW")->GetModel()->GetScale().y / m_warningCount;
	
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
