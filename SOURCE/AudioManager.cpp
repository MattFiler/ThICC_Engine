#include "pch.h"
#include "AudioManager.h"

#include <fstream>

AudioManager::AudioManager()
{
	m_audEngine = std::unique_ptr<DirectX::AudioEngine>(new DirectX::AudioEngine);

	//Read in track config
	std::fstream i(m_filepaths.generateConfigFilepath("SOUNDS_CONFIG", m_filepaths.CONFIG));
	m_sound_configs << i;

	addToSoundsList(m_sound_configs["menu_sounds"]["MENU_LOOP"], "MENU_LOOP");
	addToSoundsList(m_sound_configs["menu_sounds"]["TITLE_START"], "TITLE_START");
	addToSoundsList(m_sound_configs["menu_sounds"]["TITLE_LOOP"], "TITLE_LOOP");
	addToSoundsList(m_sound_configs["misc_sounds"]["COURSE_INTRO"], "COURSE_INTRO");
	addToSoundsList(m_sound_configs["misc_sounds"]["PRE_COUNTDOWN"], "PRE_COUNTDOWN");
	addToSoundsList(m_sound_configs["misc_sounds"]["COUNTDOWN"], "COUNTDOWN");
	addToSoundsList(m_sound_configs["misc_sounds"]["ITEMBOX_HIT"], "ITEMBOX_HIT");
	addToSoundsList(m_sound_configs["misc_sounds"]["FINAL_LAP_SOUND"], "FINAL_LAP_SOUND");
	addToSoundsList(m_sound_configs["item_sounds"]["STAR"], "STAR_SOUND");
}

void AudioManager::addToSoundsList(const std::string& asset, const std::string& _tag)
{
	//std::string filepath = "DATA/SOUNDS/" + asset + ".JSON";
	std::string filepath = m_filepaths.generateConfigFilepath(asset, m_filepaths.SOUND);
	std::fstream j;
	j.open(filepath);
	if (!j.is_open())
	{
		return;
	}
	m_sound_info << j;

	Sound* new_sound = new Sound(m_audEngine.get(), m_sound_info["asset_name"]);
	new_sound->SetLoop(m_sound_info["is_looping"]);
	new_sound->SetVolume(m_sound_info["volume"]);
	new_sound->SetPitch(m_sound_info["pitch"]);
	new_sound->SetPan(m_sound_info["pan"]);
	new_sound->SetTag(_tag);
	m_Sounds.push_back(new_sound);
	m_sound_info.clear();
}

void AudioManager::clearTrackSounds()
{
	for (size_t i = 0; i < m_Sounds.size(); i++)
	{
		if (m_Sounds[i]->GetTag() == "TRACK_START")
		{
			m_Sounds.erase(m_Sounds.begin() + i);
		}
	}
	for (size_t i = 0; i < m_Sounds.size(); i++)
	{
		if (m_Sounds[i]->GetTag() == "TRACK_LOOP")
		{
			m_Sounds.erase(m_Sounds.begin() + i);
		}
	}
	for (size_t i = 0; i < m_Sounds.size(); i++)
	{
		if (m_Sounds[i]->GetTag() == "FINAL_LAP_START")
		{
			m_Sounds.erase(m_Sounds.begin() + i);
		}
	}
	for (size_t i = 0; i < m_Sounds.size(); i++)
	{
		if (m_Sounds[i]->GetTag() == "FINAL_LAP_LOOP")
		{
			m_Sounds.erase(m_Sounds.begin() + i);
		}
	}
}

void AudioManager::eraseElementInSoundsList(const std::string& _tag)
{
	for (size_t i = 0; i < m_Sounds.size(); i++)
	{
		if (m_Sounds[i]->GetTag() == _tag)
		{
			m_Sounds.erase(m_Sounds.begin() + i);
			return;
		}
	}
}

void AudioManager::Play(const std::string& _tag)
{
	for (std::vector<Sound *>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
	{
		if ((*it)->GetTag() == _tag)
		{
			(*it)->Play();
			return;
		}
	}
}

void AudioManager::Stop(const std::string& _tag)
{
	for (std::vector<Sound *>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
	{
		if ((*it)->GetTag() == _tag)
		{
			(*it)->Stop();
			return;
		}
	}
}

void AudioManager::StopAll()
{
	for (std::vector<Sound *>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
	{
		(*it)->Stop();
	}
}

void AudioManager::Pause(const std::string& _tag)
{
	for (std::vector<Sound *>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
	{
		if ((*it)->GetTag() == _tag)
		{
			(*it)->Pause();
			return;
		}
	}
}

void AudioManager::Resume(const std::string& _tag)
{
	for (std::vector<Sound *>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
	{
		if ((*it)->GetTag() == _tag)
		{
			(*it)->Play();
			(*it)->SetPause(false);
		}
	}
}

void AudioManager::Resume(size_t i)
{
	m_Sounds[i]->Play();
	m_Sounds[i]->SetPause(false);
}

void AudioManager::Pause(size_t i)
{
	m_Sounds[i]->Pause();
	m_Sounds[i]->SetPause(true);
}

bool AudioManager::IsPlaying(size_t i)
{
	return m_Sounds[i]->IsPlaying();
}

bool AudioManager::Paused(size_t i)
{
	return m_Sounds[i]->IsPaused();
}

Sound* AudioManager::GetSound(const std::string& _tag)
{
	for (std::vector<Sound *>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
	{
		if ((*it)->GetTag() == _tag)
		{
			return (*it);
		}
	}
	return nullptr;
}
