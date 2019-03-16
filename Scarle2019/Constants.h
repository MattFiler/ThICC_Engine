#pragma once
enum Scenes
{
	MENUSCENE,
	GAMESCENE,

	SCENE_COUNT // must always be last!
};

enum States
{
	NOSTATE = -1 ,
	PAUSED = 0,

	//MENU
	LOBBY					= 1,
	START_GAME				= 2,
	CHARECTER_SELECTION		= 3,
	TRACK_SELECTION			= 4,

	//Game
	RACE_START				= 5,
	RACE_OVER				= 6
};