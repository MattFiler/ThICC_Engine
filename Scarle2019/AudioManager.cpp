#include "pch.h"
#include "AudioManager.h"

AudioManager::AudioManager()
{
	m_audEngine = std::unique_ptr<DirectX::AudioEngine>(new DirectX::AudioEngine);

	for (int i = 0; i <= (int)SOUNDS_MENU::INTRO_MUSIC; i++)
	{
		Sound* new_sound = new Sound(m_audEngine.get(), menuSounds[i]);
		m_menuSounds.push_back(new_sound);
	}
	for (int i = 0; i <= (int)SOUNDS_GAME::MKS_GAME; i++)
	{
		Sound* new_sound = new Sound(m_audEngine.get(), gameSounds[i]);
		if (i == 1)
			new_sound->SetLoop(true);
		m_gameSounds.push_back(new_sound);
	}
	for (int i = 0; i <= (int)SOUNDS_CHARACTER::WALUIGI; i++)
	{
		Sound* new_sound = new Sound(m_audEngine.get(), characterSounds[i]);
		m_characterSounds.push_back(new_sound);
	}
	for (int i = 0; i <= (int)SOUNDS_MISC::ITEM_BOX_HIT; i++)
	{
		Sound* new_sound = new Sound(m_audEngine.get(), miscSounds[i]);
		m_miscSounds.push_back(new_sound);
	}

}


void AudioManager::Play(SOUND_TYPE type, int i)
{
	switch (type)
	{
	case SOUND_TYPE::MENU:
		m_menuSounds[i]->Play();
		break;
	case SOUND_TYPE::GAME:
		m_gameSounds[i]->Play();
		break;
	case SOUND_TYPE::CHARACTER_SEL:
		m_characterSounds[i]->Play();
		break;
	case SOUND_TYPE::MISC:
		m_miscSounds[i]->Play();
		break;
	}
}

Sound* AudioManager::GetSound(SOUND_TYPE type, int i) 
{
	switch (type)
	{
	case SOUND_TYPE::MENU:
		return m_menuSounds[i];
		break;
	case SOUND_TYPE::GAME:
		return m_gameSounds[i];
		break;
	case SOUND_TYPE::CHARACTER_SEL:
		return m_characterSounds[i];
		break;
	case SOUND_TYPE::MISC:
		return m_miscSounds[i];
		break;
	}
	return nullptr;
}
