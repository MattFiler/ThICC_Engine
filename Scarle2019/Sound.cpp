#include "pch.h"
#include "Sound.h"

Sound::Sound(AudioEngine* _audEngine, string _filename)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	string fullpath = m_filepath.generateFilepath(_filename, m_filepath.SOUND);
	std::wstring wFilename = converter.from_bytes(fullpath.c_str());

	m_sfx = std::make_unique<SoundEffect>(_audEngine, wFilename.c_str());

}


Sound::~Sound()
{
	m_sfx.reset();
}

void Sound::Play()
{
	if (m_sfx)
	{
		if (loop)
		{
			if (m_playing)
			{
				m_loop->Stop(true);
			}
			else
			{
				m_loop->Play(true);
			}
			m_playing = !m_playing;
		}
		else
		{
			m_sfx->Play(m_volume, m_pitch, m_pan);
		}
	}
}


/*Only used for looping sounds*/
void Sound::Stop()
{
	if (m_sfx)
	{
		if (loop)
		{
			m_loop->Stop(true);
		}
	}
}

void Sound::Tick(GameStateData* _GSD)
{
	if (m_loop)
	{
		m_loop->SetVolume(m_volume);
		m_loop->SetPitch(m_pitch);
		m_loop->SetPan(m_pan);
		m_loop->Play(m_playing);
	}
}

void Sound::SetLoop(bool _loop) { 
	m_loop = m_sfx->CreateInstance();
	loop = _loop;
}
