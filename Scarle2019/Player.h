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
	int GetWaypoint() { return waypoint; }
	int GetRanking() { return ranking; }
	int GetLap() { return lap; }
	Text2D* GetRankingText() { return text_position; }
	Text2D* GetLapText() { return text_lap; }

	void SetWaypoint(int _waypoint) { waypoint = _waypoint; }
	void SetLap(int _lap) { lap = _lap; }
	void SetRanking(int _position) { ranking = _position; }


protected:
	GamePad* m_gamePad;
	int m_playerID = 0;
private:

	RenderData* m_RD;
	KeybindManager m_keymindManager;
	Matrix m_savedMatrix;
	Vector m_savedVel;
	Vector m_savedGravVel;
	int ranking = 0;
	int waypoint = 0;
	int lap = 1;
	Text2D *text_position = nullptr;
	Text2D *text_lap = nullptr;
};