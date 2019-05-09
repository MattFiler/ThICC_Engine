#include "pch.h"
#include "Star.h"
#include "Player.h"
#include "GameStateData.h"
#include "AudioManager.h"

Star::Star()
{
	InitStarData();
	m_shouldDespawn = false;
}

void Star::InitStarData()
{
	std::ifstream i("DATA/CONFIGS/ITEM_CONFIG.JSON");
	m_itemData << i;
	m_maxDuration = (float)m_itemData["STAR"]["info"]["duration"];
	m_moveSpeed = (float)m_itemData["STAR"]["info"]["boost"]["move_speed"];
	m_turnSpeed = (float)m_itemData["STAR"]["info"]["boost"]["turn_speed"];
	m_playerDrag = (float)m_itemData["STAR"]["info"]["boost"]["player_drag"];
}

void Star::Tick()
{
	Item::Tick();

	if (m_itemUsed)
	{
		m_elapsedTime += Locator::getGSD()->m_dt;
		if (m_elapsedTime >= m_maxDuration)
		{
			m_player->setInvicible(false);
			m_player->UseGroundTypes(true);
			m_player->GetControlledMovement()->ResetMoveSpeed();
			m_player->GetControlledMovement()->ResetTurnSpeed();

			//for (size_t i = 0; i < Locator::getAudio()->SoundListSize(); i++)
			//{
			//	if (Locator::getAudio()->Paused(i) && (Locator::getAudio()->GetSound(i)->GetTag() == "TRACK_LOOP" || Locator::getAudio()->GetSound(i)->GetTag() == "FINAL_LAP_LOOP"))
			//	{
			//		Locator::getAudio()->Resume(i);
			//	}
			//}
			//Locator::getAudio()->Stop("STAR_SOUND");
			FlagForDestoy();
		}
	}
}

void Star::Use(Player * player, bool _altUse)
{
	setItemInUse(player);
	m_trailingPlayerImmunity = true;
	m_player->GetControlledMovement()->SetMoveSpeed(m_moveSpeed);
	m_player->GetControlledMovement()->SetMoveSpeed(m_turnSpeed);

	m_player->SetDrag(m_playerDrag);
	m_player->setInvicible(true);
	m_player->UseGroundTypes(false);
	//for (size_t i = 0; i < Locator::getAudio()->SoundListSize(); i++)
	//{
	//	if (Locator::getAudio()->IsPlaying(i) && (Locator::getAudio()->GetSound(i)->GetTag() == "TRACK_LOOP" || Locator::getAudio()->GetSound(i)->GetTag() == "FINAL_LAP_LOOP"))
	//	{
	//		Locator::getAudio()->Pause(i);
	//	}
	//}
	//Locator::getAudio()->Play("STAR_SOUND");
}
