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
	NOSTATE = -1,
	PAUSED = 0,

	//MENU
	LOBBY = 1,
	START_GAME,
	CHARECTER_SELECTION,
	TRACK_SELECTION,

	//Game
	RACE_START,
	RACE_OVER
};

enum ItemType
{
	NONE = -1,
	BANANA = 0,
	GREEN_SHELL,
	RED_SHELL,
	MUSHROOM
};