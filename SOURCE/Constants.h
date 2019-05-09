#pragma once
#include <vector>

//Current engine file config - this identifies the up-to-date binary signature. 
//We can optionally add handles for older versions per-config.
struct ThICC_File {
	const static int ThICC_FILE_VERSION;
	const static std::string ThICC_FILE_IDENTIFIER;
};

/*

	!!!		DO NOT REORDER ANY OF THESE ENUMS!		!!!

*/

enum Scenes
{
	MENUSCENE,
	LOADINGSCENE,
	DEBUG_LIGHTINGTEST,
	ATTRACT_STATE,
	GAMESCENE, // must be second to last as we dynamically extend from this position

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
	GREEN_SHELL = 0,
	RED_SHELL,
	BANANA,
	FAKE_BOX,
	MUSHROOM,
	MUSHROOM_3X,
	BOMB,
	BLUE_SHELL,
	LIGHTNING_BOLT,
	STAR,
	MUSHROOM_UNLIMITED,
	MUSHROOM_GIANT,
	SQUID,
	POW,
	LIGHTNING_CLOUD,
	BULLET_BILL,
	GREEN_SHELL_3X,
	RED_SHELL_3X,
	BANANA_3X,

	PLACEHOLDER //This isn't an item - is used as the placeholder UI sprite. Do not call unless for that use case.
};


struct ItemBoxConfig {
	static double respawn_time;
};

// camera type

enum class CameraType : int
{
	FOLLOW = 0,
	BACK_FACING = 1,
	FIRST = 2,
	INDEPENDENT = 3,
	ORBIT = 4,
	CINEMATIC = 5,
#ifdef _DEBUG
	DEBUG_CAM = 6,
#endif // DEBUG
};


// sound
// sound catergories
enum class SoundType : int
{
	MENU = 0,
	GAME = 1,
	CHARACTER = 2,
	MISC = 3,
	ITEMS = 4
};

// game sounds
enum class GameSounds : int
{
	MKS_START = 0,
	MKS_GAME = 1,
	MKS_FL_START = 2,
	MKS_FL_GAME = 3
};

// character sounds
enum class CharacterSounds : int
{
	MARIO = 0,
	BOWSER = 1,
	PEACH = 2,
	WALUIGI = 3,
};

// menu sounds
enum class MenuSounds : int
{
	MENU_LOOP = 0,
	TITLE_START = 1,
	TTLE_LOOP = 2,
};

// misc sounds
enum class MiscSounds : int
{
	INTRO_MUSIC = 0,
	PRE_COUNTDOWN = 1,
	COUNTDOWN = 2,
	ITEM_BOX_HIT = 3,
	FINAL_LAP_IND = 4,
};

enum class ItemSounds : int
{
	STAR_SOUND = 0,
	LIGHTNING_SOUND = 1,
};