#pragma once
#include "Sound.h"

//GEP:: Extends the basic sound to make a looing ambient sound
// Info about it in this wiki tutorial, plus more info about the base "Audio Engine"
//https://github.com/Microsoft/DirectXTK/wiki/Adding-audio-to-your-project

class Loop :
	public Sound
{
public:
	Loop(AudioEngine* _audEngine, string _filename);
	~Loop();

	virtual void Tick(GameStateData* _GSD);

	virtual void Play();

	bool GetPlaying() { return m_playing; }
	void TogglePlaying() { m_playing = !m_playing; }
	void SetPlaying(bool _playing) { m_playing = _playing; }

protected:
	std::unique_ptr<DirectX::SoundEffectInstance> m_loop;
	bool m_playing = false;
};

