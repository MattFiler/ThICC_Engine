#pragma once
#include "TrackMagnet.h"
#include "KeybindManager.h"
#include "Banana.h"
#include "GreenShell.h"
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
	int getCurrentWaypoint() { return current_waypoint; }
	Text2D* getPosition() { return position; }

	void setCurrentWaypoint(int waypoint) { current_waypoint = waypoint; }
	void setGamePad(bool _state);
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
	Vector m_savedGravDir;
	int current_position = 0;
	int current_waypoint = 0;
	int next_waypoint = 0;
	Text2D *position = nullptr;
	bool m_controlsActive = false;
};