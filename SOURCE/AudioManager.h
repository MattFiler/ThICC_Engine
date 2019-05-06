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

	void addToSoundsList(const std::string&asset, const std::string& _tag);

	void clearTrackSounds();

	void eraseElementInSoundsList(const std::string& _tag);

	void Play(const std::string& _tag);
	void Stop(const std::string& _tag);
	void Pause(const std::string& _tag);
	Sound* GetSound(const std::string& _tag);

private:

	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	GameFilepaths m_filepaths;

	// all the sounds
	std::vector<Sound*> m_Sounds;
};