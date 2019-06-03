#include "pch.h"
#include "LightningBolt.h"
#include "Player.h"


LightningBolt::LightningBolt(std::vector<Player*> _players) : m_players(_players)
{
	m_shouldDespawn = false;
	m_baseGrowthData.m_scaleState = ItemGrowthData::SHRINK;
	m_baseGrowthData.m_scaleMulti = 0.7;
	m_baseGrowthData.m_shrinkDuration = 0.5;
	m_baseGrowthData.m_growthDuration = 0.5;
	m_playerSpinRev = 2;
	m_playerSpinDuration = 0.5;
	m_playerMoveSpeed = 20;
	m_playerTurnSpeed = 30;
}

void LightningBolt::Tick()
{
	if (m_itemUsed)
	{
		for(StrikeData& data : m_strikeDatas)
		{ 
			if (m_finishedCount >= m_strikeDatas.size())
			{
				FlagForDestoy();
				break;
			}
			else if (data.m_finished)
			{
				continue;
			}
			else if (data.m_player->isInvincible() || data.m_player->IsRespawning())
			{
				data.m_player->GetAnimController()->SwitchModelSet(data.m_player->IsGliding() ? "gliding" : "default");
				data.m_growthData.m_scaleState = ItemGrowthData::GROW;
			}

			switch (data.m_growthData.m_scaleState)
			{
			case ItemGrowthData::SHRINK:
				if (!data.m_growthData.m_shrinking)
				{
					std::string gliding = data.m_player->IsGliding() ? " Gliding" : "";
					data.m_player->GetAnimController()->SwitchModelSet("Lightning" + gliding);
					data.m_player->DropItems();
					data.m_player->Spin(m_playerSpinRev, m_playerSpinDuration);
					data.m_player->Scale(data.m_growthData.m_shrinkScale, data.m_growthData.m_shrinkDuration);
					data.m_growthData.m_shrinking = true;
				}
				data.m_player->SetScale(data.m_player->GetAnimController()->GetScaleOffset());
				/*data.m_player->GetAnimController()->GetModelFromSet(data.m_player->GetAnimController()->GetCurrentSet(), "Lightning")->GetModel()->SetOri(data.m_lightningRot);
				data.m_player->GetAnimController()->GetModelFromSet(data.m_player->GetAnimController()->GetCurrentSet(), "Lightning")->GetModel()->UpdateWorld();*/

				if (data.m_player->GetAnimController()->FinishedScale())
				{
					data.m_player->GetAnimController()->SwitchModelSet(data.m_player->IsGliding() ? "gliding" : "default");
					data.m_player->GetControlledMovement()->SetMoveSpeed(m_playerMoveSpeed);
					data.m_player->GetControlledMovement()->SetTurnSpeed(m_playerTurnSpeed);
					data.m_growthData.m_scaleState = ItemGrowthData::MAINTIAIN;
				}

				break;

			case ItemGrowthData::MAINTIAIN:

				data.m_growthData.m_sizeChangeTimeElapsed += Locator::getGSD()->m_dt;
				if (data.m_growthData.m_sizeChangeTimeElapsed >= data.m_growthData.m_sizeChangeDuration)
				{
					data.m_growthData.m_scaleState = ItemGrowthData::GROW;
				}

				break;
			case ItemGrowthData::GROW:
				if (!data.m_growthData.m_growing)
				{
					data.m_player->Scale(data.m_growthData.m_growScale, data.m_growthData.m_growthDuration);
					data.m_growthData.m_growing = true;
				}
				data.m_player->SetScale(data.m_player->GetAnimController()->GetScaleOffset());

				if (data.m_player->GetAnimController()->FinishedScale())
				{
					data.m_player->GetControlledMovement()->ResetMoveSpeed();
					data.m_player->GetControlledMovement()->ResetTurnSpeed();
					++m_finishedCount;
					data.m_finished = true;
				}
				break;
			}


		}
	}
}

void LightningBolt::Use(Player * _player, bool _altUse)
{
	setItemInUse(_player);
	m_trailingPlayerImmunity = true;

	FindPlayers();
}

void LightningBolt::FindPlayers()
{
	for (auto it = m_players.begin(); it != m_players.end();)
	{
		if ((*it)->GetRanking() >= m_player->GetRanking())
		{
			it = m_players.erase(it);
		}
		else
		{
			StrikeData strikeData = StrikeData();
			strikeData.m_player = *it;
			strikeData.m_growthData = m_baseGrowthData;
			strikeData.m_growthData.m_sizeChangeDuration = m_maxSizeChangeDuration - (*it)->GetRanking();
			strikeData.m_growthData.m_growScale = (*it)->GetScale();
			strikeData.m_growthData.m_shrinkScale = strikeData.m_growthData.m_growScale * strikeData.m_growthData.m_scaleMulti;
			strikeData.m_lightningRot = (*it)->GetAnimController()->GetModelFromSet("Lightning", "Lightning")->GetModel()->GetOri();
			m_strikeDatas.push_back(strikeData);
			++it;
		}
	}

	m_players.clear();
}
