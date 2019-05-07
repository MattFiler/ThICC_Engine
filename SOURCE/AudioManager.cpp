#include "pch.h"
#include "AudioManager.h"
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

AudioManager::AudioManager()
{
	m_audEngine = std::unique_ptr<DirectX::AudioEngine>(new DirectX::AudioEngine);

	//Read in track config
	std::ifstream i(m_filepaths.generateConfigFilepath("SOUNDS_CONFIG", m_filepaths.CONFIG));
	json m_sound_configs;
	m_sound_configs << i;

	addToSoundsList(m_sound_configs["menu_sounds"]["MENU_LOOP"], "MENU_LOOP");
	addToSoundsList(m_sound_configs["menu_sounds"]["TITLE_START"], "TITLE_START");
	addToSoundsList(m_sound_configs["menu_sounds"]["TITLE_LOOP"], "TITLE_LOOP");
	addToSoundsList(m_sound_configs["misc_sounds"]["COURSE_INTRO"], "COURSE_INTRO");
	addToSoundsList(m_sound_configs["misc_sounds"]["PRE_COUNTDOWN"], "PRE_COUNTDOWN");
	addToSoundsList(m_sound_configs["misc_sounds"]["COUNTDOWN"], "COUNTDOWN");
	addToSoundsList(m_sound_configs["misc_sounds"]["ITEMBOX_HIT"], "ITEMBOX_HIT");
	addToSoundsList(m_sound_configs["misc_sounds"]["FINAL_LAP_SOUND"], "FINAL_LAP_SOUND");
	addToSoundsList(m_sound_configs["item_sounds"]["STAR_SOUND"], "STAR_SOUND");
	addToSoundsList(m_sound_configs["item_sounds"]["LIGHTNING_SOUND"], "LIGHTNING_SOUND");

}

void AudioManager::addToSoundsList(const std::string& asset, const std::string& _tag)
{
	std::ifstream j(m_filepaths.generateConfigFilepath(asset, m_filepaths.SOUND));
	json m_config;
	m_config << j;

	Sound* new_sound = new Sound(m_audEngine.get(), m_config["asset_name"]);
	new_sound->SetLoop(m_config["is_looping"]);
	new_sound->SetVolume(m_config["volume"]);
	new_sound->SetPitch(m_config["pitch"]);
	new_sound->SetPan(m_config["pan"]);
	new_sound->SetTag(_tag);
	m_Sounds.push_back(new_sound);
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
