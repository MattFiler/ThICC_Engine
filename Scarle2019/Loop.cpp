#include "pch.h"
#include "Loop.h"


Loop::Loop(AudioEngine* _audEngine, string _filename, bool _loop) //:Sound(_audEngine, _filename)
{
	if (m_sfx)
	{
		m_loop = m_sfx->CreateInstance();
	}
}


Loop::~Loop()
{
	m_loop->Stop(true);
	m_loop.reset();
}

void Loop::Play()
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

//
//void Loop::Tick(GameStateData* _GSD)
//{
//
//	//m_loop->SetVolume(m_volume);
//	//m_loop->SetPitch(m_pitch);
//	//m_loop->SetPan(m_pan);
//	//m_loop->Play(m_playing);
//}
