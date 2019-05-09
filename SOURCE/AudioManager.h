#pragma once

#include "Sound.h"
#include "Constants.h"
#include "GameFilepaths.h"

#include <Audio.h>
#include <json.hpp>
using json = nlohmann::json;

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
	void StopAll();
	void Pause(const std::string& _tag);
	void Pause(size_t i);
	bool IsPlaying(size_t i);
	bool Paused(size_t i);
	void Resume(const std::string& _tag);
	void Resume(size_t i);
	size_t SoundListSize() { return m_Sounds.size(); }
	Sound* GetSound(const std::string& _tag);
	Sound* GetSound(size_t i) { return m_Sounds[i]; }

private:

	std::unique_ptr<DirectX::AudioEngine> m_audEngine;

	GameFilepaths m_filepaths;
	json m_sound_configs;
	json m_sound_info;

	// all the sounds
	std::vector<Sound*> m_Sounds;
};