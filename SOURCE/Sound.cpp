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
	CreateInst();
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
		if (m_soundInst)
		{

			if (loop)
			{
				if (m_playing)
				{
					m_soundInst->Stop(true);
				}
				else
				{
					m_soundInst->Play(true);
				}
				m_playing = !m_playing;
			}
			else
			{
				m_soundInst->SetVolume(m_volume);
				m_soundInst->SetPitch(m_pitch);
				m_soundInst->SetPan(m_pan);
				m_soundInst->Play(loop);
			}
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
	if (m_sfx && m_soundInst)
	{
		m_soundInst->Stop(true);
		m_playing = false;
	}
}

/* Update sound (only for loops) */
void Sound::Tick()
{
	if (m_soundInst)
	{
		m_soundInst->SetVolume(m_volume);
		m_soundInst->SetPitch(m_pitch);
		m_soundInst->SetPan(m_pan);
		m_soundInst->Play(m_playing);
	}
}

/* Set looping */
void Sound::SetLoop(bool _loop) {
	loop = _loop;
	//m_soundInst = m_sfx->CreateInstance();
}

void Sound::CreateInst() {
	m_soundInst = m_sfx->CreateInstance();
}

void Sound::Pause()
{
	if (m_sfx && m_soundInst)
	{
		m_soundInst->Pause();
	}
}

void Sound::Resume()
{
	if (m_sfx && m_soundInst)
	{
		m_soundInst->Resume();
	}
}
