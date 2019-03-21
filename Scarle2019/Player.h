#pragma once
#include "TrackMagnet.h"
#include "KeybindManager.h"
#include "Banana.h"
#include "GreenShell.h"
#include "Mushroom.h"
#include "Constants.h"
#include <functional>

//=================================================================
//Base Player Class (i.e. a model GO3D the player controls)
//=================================================================

class Player : public TrackMagnet
{

public:
	Player(string _filename, int _playerID, std::function<Item*(ItemType)> _createItemFunction);
	~Player();

	virtual void Tick() override;

	int GetWaypoint() { return m_waypoint; }
	int GetRanking() { return m_ranking; }
	int GetLap() { return m_lap; }
	bool GetFinished() { return m_finished; }
	Text2D* GetRankingText() { return m_textRanking; }
	Text2D* GetLapText() { return m_textLap; }
	Text2D* GetCountdown() { return m_textCountdown; }
	Text2D* GetFinishOrder() { return m_textFinishOrder; }
	ImageGO2D* GetItemImg() { return m_imgItem; }
	std::vector<std::string> GetOrderIndicator() { return m_orderIndicator; }

	void SetWaypoint(int _waypoint) { m_waypoint = _waypoint; }
	void SetLap(int _lap) { m_lap = _lap; }
	void SetRanking(int _position) { m_ranking = _position; }
	void setGamePad(bool _state);
	void SetFinished(bool _finished) { m_finished = _finished; }

	/* Inventory Management */
	ItemType getActiveItem() { return active_item; };
	void setActiveItem(ItemType _item);
	ItemType getItemInInventory() { return inventory_item; };
	void setItemInInventory(ItemType _item);
	void TrailItem();
	void SpawnItem(ItemType type);
	void ReleaseItem();

protected:
	int m_playerID = 0;

private:
	std::function<Item*(ItemType)> CreateItem;
	void movement();


	RenderData* m_RD;
	KeybindManager m_keymindManager;
	Matrix m_savedMatrix;
	Vector m_savedPos;
	Vector m_savedVel;
	Vector m_savedGravVel;
	int m_ranking = 0;
	int m_waypoint = 0;
	int m_lap = 1;
	Text2D *m_textRanking = nullptr;
	Text2D *m_textLap = nullptr;
	Text2D *m_textCountdown = nullptr;
	Text2D *m_textFinishOrder = nullptr;
	Vector m_savedGravDir;
	bool m_finished = false;

	std::vector<std::string> m_orderIndicator{ "st","nd", "rd", "th" };

	// Player items:
	//	A player can have an ACTIVE item (e.g. holding a banana behind themselves) AND also an INVENTORY item.
	//	An inventory item is an item in the UI square which is held back ready to be used.
	//	If there is an item in inventory, the player cannot	aquire another.
	//	If there is an active item, the player can acquire one to their inventory.
	//	When an item is used it should move from inventory to active (or be immediately used, etc - some items differ).
	ItemType active_item = ItemType::NONE;
	ItemType inventory_item = ItemType::NONE;
	
	ImageGO2D *m_imgItem = nullptr;
	Item* m_trailingItem = nullptr;
	bool  m_isTrailing = false;
	
	bool m_controlsActive = false;
};