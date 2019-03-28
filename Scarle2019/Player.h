#pragma once
#include "TrackMagnet.h"
#include "KeybindManager.h"
#include "Banana.h"
#include "GreenShell.h"
#include "Mushroom.h"
#include "Constants.h"
#include "AnimationMesh.h"
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
	virtual void Render() override;

	int GetWaypoint() { return m_waypoint; }
	int GetRanking() { return m_ranking; }
	int GetLap() { return m_lap; }
	bool GetFinished() { return m_finished; }
	Text2D* GetRankingText() { return m_textRanking; }
	Text2D* GetLapText() { return m_textLap; }
	Text2D* GetCountdown() { return m_textCountdown; }
	Text2D* GetFinishOrder() { return m_textFinishOrder; }
	ImageGO2D* GetItemImg() { return m_imgItem; }
	void SetItemPos(Vector2 _pos) { m_itemPos = _pos; }
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

	void Spin(int _revolutions, float _duration) { m_displayedMesh->Spin(_revolutions, _duration); };
	void Flip(int _revolutions, float _duration) { m_displayedMesh->Flip(_revolutions, _duration); };
	void Jump(float _jumpHeight, float _duration) { m_displayedMesh->Jump(_jumpHeight, _duration); };

protected:
	int m_playerID = 0;

private:
	void Animations();
	std::function<Item*(ItemType)> CreateItem;
	void movement();

	void EndDrift();

	double m_timeTurning = 0;
	float m_maxTurnRateMutliplier = 2.3f;
	float m_maxDriftTurnMutliplier = 4.2f;
	double m_timeForMaxTurn = 4;
	double m_timeForMaxDrift = 8;
	bool m_drifting = false;
	bool m_driftingRight = false;
	float m_driftBoost = 300;

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
	
	Vector2 m_itemPos = Vector2(0, 0); //temp gpu fix 
	ImageGO2D *m_imgItem = nullptr;

	Item* m_trailingItem = nullptr;
	bool  m_isTrailing = false;
	
	bool m_controlsActive = false;

	std::unique_ptr<AnimationMesh> m_displayedMesh = nullptr;
	Vector3 m_targetAnimPosOffset = Vector3::Zero;
	Vector3 m_targetAnimRotOffset = Vector3::Zero;

};