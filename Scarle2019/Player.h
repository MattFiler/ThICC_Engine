#pragma once
#include "TrackMagnet.h"
#include "KeybindManager.h"

//=================================================================
//Base Player Class (i.e. a model GO3D the player controls)
//=================================================================

class Player : public TrackMagnet
{

public:
	Player(RenderData* _RD, string _filename, int _playerID, GamePad &_gamePad);
	~Player();

	virtual void Tick(GameStateData* _GSD) override;
	int getCurrentWaypoint() { return current_waypoint; }
	Text2D* getPosition() { return position; }

	void setCurrentWaypoint(int waypoint) { current_waypoint = waypoint; }



protected:
	GamePad* m_gamePad;
	int m_playerID = 0;
private:

	RenderData* m_RD;
	KeybindManager m_keymindManager;
	Matrix m_savedMatrix;
	Vector m_savedVel;
	Vector m_savedGravVel;
	int current_position = 0;
	int current_waypoint = 0;
	int next_waypoint = 0;
	Text2D *position = nullptr;
};