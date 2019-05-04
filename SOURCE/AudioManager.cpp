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

	addToSoundsList(m_sound_configs["menu_sounds"]["MENU_LOOP"], SoundType::MENU);
	addToSoundsList(m_sound_configs["menu_sounds"]["TITLE_START"], SoundType::MENU);
	addToSoundsList(m_sound_configs["menu_sounds"]["TITLE_LOOP"], SoundType::MENU);
	addToSoundsList(m_sound_configs["misc_sounds"]["COURSE_INTRO"], SoundType::MISC);
	addToSoundsList(m_sound_configs["misc_sounds"]["PRE_COUNTDOWN"], SoundType::MISC);
	addToSoundsList(m_sound_configs["misc_sounds"]["COUNTDOWN"], SoundType::MISC);
	addToSoundsList(m_sound_configs["misc_sounds"]["ITEMBOX_HIT"], SoundType::MISC);
	addToSoundsList(m_sound_configs["misc_sounds"]["FINAL_LAP_SOUND"], SoundType::MISC);
	addToSoundsList(m_sound_configs["item_sounds"]["STAR_SOUND"], SoundType::ITEMS);

	//for (int i = 0; i <= (int)SOUNDS_MENU::TTLE_LOOP; i++)
	//{
	//	Sound* new_sound = new Sound(m_audEngine.get(), menuSounds[i]);
	//	if (i == 0 || i == 2)
	//		new_sound->SetLoop(true);
	//	m_menuSounds.push_back(new_sound);
	//}
	//for (int i = 0; i <= (int)SOUNDS_GAME::MKS_FL_GAME; i++)
	//{
	//	Sound* new_sound = new Sound(m_audEngine.get(), gameSounds[i]);
	//	if (i == 1)
	//		new_sound->SetLoop(true);
	//	m_gameSounds.push_back(new_sound);
	//}
	//for (int i = 0; i <= (int)SOUNDS_CHARACTER::WALUIGI; i++)
	//{
	//	Sound* new_sound = new Sound(m_audEngine.get(), characterSounds[i]);
	//	m_characterSounds.push_back(new_sound);
	//}
	//for (int i = 0; i <= (int)SOUNDS_MISC::FINAL_LAP_IND; i++)
	//{
	//	Sound* new_sound = new Sound(m_audEngine.get(), miscSounds[i]);
	//	m_miscSounds.push_back(new_sound);
	//}

}

void AudioManager::addToSoundsList(std::string asset, SoundType type)
{
	std::ifstream j("DATA/SOUNDS/" + asset + ".JSON");
	json m_config;
	m_config << j;

	Sound* new_sound = new Sound(m_audEngine.get(), m_config["asset_name"]);
	new_sound->SetLoop(m_config["is_looping"]);
	new_sound->SetVolume(m_config["volume"]);
	new_sound->SetPitch(m_config["pitch"]);
	new_sound->SetPan(m_config["pan"]);

	auto soundssss = new_sound;
	switch (type)
	{
	case SoundType::MENU:
		m_menuSounds.emplace_back(new_sound);
		break;
	case SoundType::GAME:
		m_gameSounds.emplace_back(new_sound);
		break;
	case SoundType::CHARACTER:
		m_characterSounds.emplace_back(new_sound);
		break;
	case SoundType::MISC:
		m_miscSounds.emplace_back(new_sound);
		break;
	case SoundType::ITEMS:
		m_itemSounds.emplace_back(new_sound);
		break;
	}
}

void AudioManager::clearSoundsList(SoundType type)
{
	switch (type)
	{
	case SoundType::MENU:
		m_menuSounds.clear();
		break;
	case SoundType::GAME:
		m_gameSounds.clear();
		break;
	case SoundType::CHARACTER:
		m_characterSounds.clear();
		break;
	case SoundType::MISC:
		m_miscSounds.clear();
		break;
	case SoundType::ITEMS:
		m_itemSounds.clear();
		break;
	}

}

void AudioManager::eraseElementInSoundsList(SoundType type, int i)
{
	switch (type)
	{
	case SoundType::MENU:
		m_menuSounds.erase(m_menuSounds.begin() + i);
		break;
	case SoundType::GAME:
		m_gameSounds.erase(m_menuSounds.begin() + i);
		break;
	case SoundType::CHARACTER:
		m_characterSounds.erase(m_menuSounds.begin() + i);
		break;
	case SoundType::MISC:
		m_miscSounds.erase(m_menuSounds.begin() + i);
		break;
	case SoundType::ITEMS:
		m_itemSounds.erase(m_menuSounds.begin() + i);
		break;
	}
}

void AudioManager::Play(SoundType type, int i)
{
	switch (type)
	{
	case SoundType::MENU:
		m_menuSounds[i]->Play();
		break;
	case SoundType::GAME:
		m_gameSounds[i]->Play();
		break;
	case SoundType::CHARACTER:
		m_characterSounds[i]->Play();
		break;
	case SoundType::MISC:
		m_miscSounds[i]->Play();
		break;
	case SoundType::ITEMS:
		m_itemSounds[i]->Play();
		break;
	}
}

void AudioManager::Stop(SoundType type, int i)
{
	switch (type)
	{
	case SoundType::MENU:
		m_menuSounds[i]->Stop();
		break;
	case SoundType::GAME:
		m_gameSounds[i]->Stop();
		break;
	case SoundType::CHARACTER:
		m_characterSounds[i]->Stop();
		break;
	case SoundType::MISC:
		m_miscSounds[i]->Stop();
		break;
	case SoundType::ITEMS:
		m_itemSounds[i]->Stop();
		break;
	}
}

void AudioManager::Pause(SoundType type, int i)
{
	switch (type)
	{
	case SoundType::MENU:
		m_menuSounds[i]->Pause();
		break;
	case SoundType::GAME:
		m_gameSounds[i]->Pause();
		break;
	case SoundType::CHARACTER:
		m_characterSounds[i]->Pause();
		break;
	case SoundType::MISC:
		m_miscSounds[i]->Pause();
		break;
	case SoundType::ITEMS:
		m_itemSounds[i]->Pause();
		break;
	}
}

Sound* AudioManager::GetSound(SoundType type, int i)
{
	switch (type)
	{
	case SoundType::MENU:
		return m_menuSounds[i];
	case SoundType::GAME:
		return m_gameSounds[i];
	case SoundType::CHARACTER:
		return m_characterSounds[i];
	case SoundType::MISC:
		return m_miscSounds[i];
	case SoundType::ITEMS:
		return m_itemSounds[i];
	}
	return nullptr;
}
