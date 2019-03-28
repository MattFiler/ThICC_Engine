#pragma once

#include "Constants.h"
#include "pch.h"

#include <vector>


class Player;

class GUIManager
{
public:
	GUIManager();
	~GUIManager() = default;

	void RenderPlayersGUI();
	void RenderMiniMap();

	void load();

	//setters
	void setActivePlayerItemGUI(Player& _player);
	void setActivePlayerPositionGUI(int _current_position, int _playerID);
	void setPlayerlapGUI(int _current_lap, int _playerID);
	//void setMiniMapGUI(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList1>&  _m_commandList);

private:
	//Temp Sprite Paths
	std::string m_itemSpritePaths[18]{ "ITEM_BANANA", "ITEM_BANANA_2", "ITEM_BANANA_3", "ITEM_BLUESHELL",
		"ITEM_BULLET_BILL", "ITEM_FAKE_ITEMBOX", "ITEM_GREEN_SHELL", "ITEM_GREEN_SHELL_2", "ITEM_GREEN_SHELL_3",
		"ITEM_LIGHTNING", "ITEM_MUSHROOM", "ITEM_MUSHROOM_2", "ITEM_MUSHROOM_3", "ITEM_MUSHROOM_GOLD", "ITEM_RED_SHELL", "ITEM_RED_SHELL_2", "ITEM_RED_SHELL_3",
		"ITEM_ITEM_SQUID" };
	std::string m_charecterMenuSpritepaths[5]{ "Bowser", "Mario", "Peach", "Waluigi", "Yoshi" };
	std::string m_miniMapSpritesPaths[4]{ "Track_1", "Track_1", "Track_1", "Track_1" };
	std::string m_positionSpritePaths[12]{ "1st", "2nd", "3rd", "4th", "5th", "6th", "7th", "8th", "9th", "10th", "11th", "12th" };

	ImageGO2D* m_activePlayerItemGUI[4]		= { nullptr, nullptr, nullptr, nullptr };
	ImageGO2D* m_activePlayerPositionGUI[4] = { nullptr, nullptr, nullptr, nullptr };
	Text2D* m_activePlayerLapGUI[4]			= { nullptr, nullptr, nullptr, nullptr };
	
	std::vector<ImageGO2D*> m_itemSprites;
	std::vector<ImageGO2D*> m_charecterMenuSprites;
	std::vector<ImageGO2D*> m_miniMapCharecterSprites;
	std::vector<ImageGO2D*> m_positionPlaceSprites;

	//Minimap, 5th camera needs to be placed over the top of the other player cameras which only renders the track? alpha blend?
	//Other option render png of track on 2d viewport and get the position of players another way.
};

