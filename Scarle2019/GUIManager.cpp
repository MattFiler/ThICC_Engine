#include "pch.h"
#include "GUIManager.h"
#include "Player.h"


GUIManager::GUIManager()
{
	for (int i = 0; i < m_itemSpritePaths->size(); ++i)
	{
		m_itemSprites.emplace_back(new ImageGO2D(m_itemSpritePaths[i]));
	}

	for (int i = 0; i < m_positionPlaceSprites.size(); ++i)
	{
		m_positionPlaceSprites.emplace_back(new ImageGO2D(m_positionSpritePaths[i]));
	}

	for (int i = 0; i < m_charecterMenuSpritepaths->size(); ++i)
	{
		m_charecterMenuSprites.emplace_back(new ImageGO2D(m_charecterMenuSpritepaths[i]));
	}

	for (int i = 0; i < m_miniMapSpritesPaths->size(); ++i)
	{
		m_miniMapCharecterSprites.emplace_back(new ImageGO2D(m_miniMapSpritesPaths[i]));
	}
}


void GUIManager::RenderPlayersGUI()
{
}

void GUIManager::RenderMiniMap()
{

}

void GUIManager::setActivePlayerItemGUI(Player& _player)
{
	m_activePlayerItemGUI[_player.get] = m_itemSprites[static_cast<int>(_current_item)];
}

void GUIManager::setActivePlayerPositionGUI(int _current_position, int _playerID)
{
	m_activePlayerPositionGUI[_playerID] = m_positionPlaceSprites[static_cast<int>(_current_position)];
}

void GUIManager::setPlayerlapGUI(int _current_lap, int _playerID)
{
	m_activePlayerLapGUI[_playerID]->SetText(std::to_string(_current_lap) + "/3");
}
 