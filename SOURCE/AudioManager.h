#pragma once

#include "Sound.h"
#include "Constants.h"
#include "GameFilepaths.h"

#include <Audio.h>

class AudioManager
{
public:

	AudioManager();
	~AudioManager() = default;

	void addToSoundsList(std::string asset, SoundType type);

	void clearSoundsList(SoundType type);

	void eraseElementInSoundsList(SoundType type, int i);

	// to pass in type of sound you want to play and which sound (cast to an int)
	void Play(SoundType type, int i);
	void Stop(SoundType type, int i);
	void Pause(SoundType type, int i);

	// to pass in type of sound you want to get and which sound (cast to an int)
	Sound* GetSound(SoundType type, int i);

private:

	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	GameFilepaths m_filepaths;

	// temp filepaths
	//std::string menuSounds[3]{ "Menu Loop", "Title Start", "Title Loop" };
	//std::string gameSounds[4]{ "Mks Start Music", "Mks Main Music", "Mks Final Lap Start", "Mks Final lap Loop" };
	//std::string characterSounds[4]{ "Mario Menu","Bowser Menu","Peach Menu","Waluigi Menu" };
	//std::string miscSounds[5]{ "Course Intro Soundtrack","Pre Countdown","Countdown","Item Box Hit", "Final Lap Indicator" };

	// all the sounds
	std::vector<Sound*> m_menuSounds;
	std::vector<Sound*> m_gameSounds;
	std::vector<Sound*> m_characterSounds;
	std::vector<Sound*> m_miscSounds;
	std::vector<Sound*> m_itemSounds;
};