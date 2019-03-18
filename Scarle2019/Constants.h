#pragma once
enum Scenes
{
	MENUSCENE,
	GAMESCENE,
	DEBUGSCENE,

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

enum ItemType
{
	BANANA = 0,
	GREEN_SHELL = 1,
	RED_SHELL = 2,
	MUSHROOM = 3
};