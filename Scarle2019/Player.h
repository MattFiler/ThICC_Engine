#pragma once
#include "TrackMagnet.h"
#include "KeybindManager.h"

//=================================================================
//Base Player Class (i.e. a model GO3D the player controls)
//=================================================================

class Player : public TrackMagnet
{

public:
	Player(string _filename, int _playerID);
	~Player();

	virtual void Tick() override;
	int GetWaypoint() { return waypoint; }
	int GetRanking() { return ranking; }
	int GetLap() { return lap; }
	Text2D* GetRankingText() { return text_position; }
	Text2D* GetLapText() { return text_lap; }
	virtual void Tick(GameStateData* _GSD) override;

	void SetWaypoint(int _waypoint) { waypoint = _waypoint; }
	void SetLap(int _lap) { lap = _lap; }
	void SetRanking(int _position) { ranking = _position; }


protected:
	int m_playerID = 0;

private:
	RenderData* m_RD;
	KeybindManager m_keymindManager;
	Matrix m_savedMatrix;
	Vector m_savedPos;
	Vector m_savedVel;
	Vector m_savedGravVel;
	int ranking = 0;
	int waypoint = 0;
	int lap = 1;
	Text2D *text_position = nullptr;
	Text2D *text_lap = nullptr;
	Vector m_savedGravDir;
};