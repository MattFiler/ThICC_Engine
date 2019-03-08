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
	START_GAME = 1,
	CHARECTER_SELECTION = 2,
	TRACK_SELECTION = 3,

	//Game
	RACE_START = 4,
	RACE_OVER = 5,
};