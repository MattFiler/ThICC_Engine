#include "pch.h"
#include "Sound.h"

/* Create */
Sound::Sound(DirectX::AudioEngine* _audEngine, std::string _filename)
{
	filename = _filename;

	//Work out filepath
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string fullpath = m_filepath.generateFilepath(_filename, m_filepath.SOUND);
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	try {
		//Load sound effect
		m_sfx = std::make_unique<DirectX::SoundEffect>(_audEngine, wFilename.c_str());
	}
	catch (...) {
		DebugText::print("Attempt to load sound '" + _filename + "' FAILED! Is the asset path correct?");
		DebugText::print(fullpath);
	}
	//m_inst = m_sfx->CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS::SoundEffectInstance_Use3D);
	m_inst = m_sfx->CreateInstance();
}

/* Destroy! */
Sound::~Sound()
{
	m_sfx.reset();
}

/* Play sound */
void Sound::Play()
{
	if (m_sfx)
	{
		//if (loop)
		//{
		//	if (m_playing)
		//	{
		//		m_loop->Stop(true);
		//	}
		//	else
		//	{
		//		m_loop->Play(true);
		//	}
		//	m_playing = !m_playing;
		//}
		//else
		//{
		//	m_sfx->Play(m_volume, m_pitch, m_pan);
		//}

		if (loop)
		{
			if (m_playing)
			{
				m_inst->Stop(true);
			}
			else
			{
				m_inst->Play(true);
			}
			m_playing = !m_playing;
		}
		else
		{
			m_inst->Play(false);
		}
	}
	else
	{
		DebugText::print("Call to play sound '" + filename + "' which was not loaded!");
	}
}

/* Stop sound (only used for looping sounds) */
void Sound::Stop()
{
	if (m_sfx)
	{
		if (loop)
		{
			m_inst->Stop(true);
			m_playing = false;
		}
		else
		{
			m_inst->Pause();
		}
	}
}

/* Update sound (only for loops) */
void Sound::Tick()
{
	if (m_loop)
	{
		//m_loop->SetVolume(m_volume);
		//m_loop->SetPitch(m_pitch);
		//m_loop->SetPan(m_pan);
		//m_loop->Play(m_playing);
		m_inst->SetVolume(m_volume);
		m_inst->SetPitch(m_pitch);
		m_inst->SetPan(m_pan);
		m_inst->Play(m_playing);
	}
}

/* Set looping */
void Sound::SetLoop(bool _loop) {
	loop = _loop;
	//m_loop = m_sfx->CreateInstance();
}

void Sound::PauseMusic()
{
	if (m_sfx)
	{
		m_inst->Pause();
		if (loop)
		{
			m_playing = false;
		}
	}
}
