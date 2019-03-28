#include "pch.h"
#include "GUIManager.h"



GUIManager::GUIManager()
{
	for (int i = 0; i < m_itemSpritePaths->size(); ++i)
	{
		m_itemSprites.emplace_back(new ImageGO2D(m_itemSpritePaths[i]));
	}
}


void GUIManager::Render(Scenes _current_scene, States _current_state)
{
	switch (_current_scene)
	{
	case Scenes::MENUSCENE:

		break;
	case Scenes::GAMESCENE:
		switch (_current_state)
		{
		case States::PLAY:
			for (int i = 0; i < 4; ++i)
			{
				m_activePlayerItemGUI[i]->Render();
				m_activePlayerPositionGUI[i]->Render();
				m_activePlayerLapGUI[i]->Render();
			}
				break;
		default:
			break;
		}
	default:
		break;
	}
}

void GUIManager::load()
{
	//Init all sprites here

	//set position of menu elements here e.g. charecter selected
	//set position of all 4 players GUI here.



}

void GUIManager::setActivePlayerItemGUI(ItemType _current_item, int _playerID)
{
	m_activePlayerItemGUI[_playerID] = m_itemSprites[static_cast<int>(_current_item)];
}

void GUIManager::setActivePlayerPositionGUI(int _current_position, int _playerID)
{
	m_activePlayerPositionGUI[_playerID] = m_positionPlaceSprites[static_cast<int>(_current_position)];
}

void GUIManager::setPlayerlapGUI(int _current_lap, int _playerID)
{
	m_activePlayerLapGUI[_playerID]->SetText(std::to_string(_current_lap) + "/3");
}
 