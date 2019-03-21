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

	int GetWaypoint() { return waypoint; }
	int GetRanking() { return ranking; }
	int GetLap() { return lap; }
	Text2D* GetRankingText() { return text_ranking; }
	Text2D* GetLapText() { return text_lap; }
	ImageGO2D* GetItemImg() { return item_img; }
	Text2D* GetCountdown() { return countdown; }

	void SetWaypoint(int _waypoint) { waypoint = _waypoint; }
	void SetLap(int _lap) { lap = _lap; }
	void SetRanking(int _position) { ranking = _position; }
	void setGamePad(bool _state);

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

	void EndDrift();

	double m_timeTurning = 0;
	float m_maxTurnRateMutliplier = 2.3f;
	float m_maxDriftTurnMutliplier = 4.2f;
	double m_timeForMaxTurn = 4;
	double m_timeForMaxDrift = 4;
	bool m_drifting = false;
	bool m_driftingRight = false;
	float m_driftBoost = 300;

	RenderData* m_RD;
	KeybindManager m_keymindManager;
	Matrix m_savedMatrix;
	Vector m_savedPos;
	Vector m_savedVel;
	Vector m_savedGravVel;
	int ranking = 0;
	int waypoint = 0;
	int lap = 1;
	Text2D *text_ranking = nullptr;
	Text2D *text_lap = nullptr;
	Text2D *countdown = nullptr;
	Vector m_savedGravDir;

	// Player items:
	//	A player can have an ACTIVE item (e.g. holding a banana behind themselves) AND also an INVENTORY item.
	//	An inventory item is an item in the UI square which is held back ready to be used.
	//	If there is an item in inventory, the player cannot	aquire another.
	//	If there is an active item, the player can acquire one to their inventory.
	//	When an item is used it should move from inventory to active (or be immediately used, etc - some items differ).
	ItemType active_item = ItemType::NONE;
	ItemType inventory_item = ItemType::NONE;
	
	ImageGO2D *item_img = nullptr;
	Item* m_trailingItem = nullptr;
	bool  m_isTrailing = false;
	
	bool m_controlsActive = false;
};