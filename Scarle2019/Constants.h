#pragma once
enum Scenes
{
	NONE = 0,
	MENUSCENE = 1,
	GAMESCENE = 2
};

enum States
{
	
	NOSTATE = -1 ,
	PAUSED = 0,

	//MENU
	START_GAME = 1,
	CHARECTER_SELECTION = 2,
	TRACK_SELECTION = 3,

	//Game
	RACE_START = 4,
	RACE_OVER = 5,
};