#pragma once
#include "Audio.h"
#include <string>
#include <codecvt>
struct GameStateData;

//GEP:: Essential sound effect stuff here
// https://github.com/Microsoft/DirectXTK/wiki/Adding-the-DirectX-Tool-Kit-for-Audio
// More involved 3D sound system seems to be in place but the wiki documentation runs out

class Sound
{
public:
	Sound(AudioEngine* _audEngine, string _filename);
	virtual ~Sound();

	virtual void Tick(GameStateData* _GSD) {};

	virtual void Play();

	void SetVolume(float _vol) { m_volume = _vol; }
	float GetVolume() { return m_volume; }

	void SetPitch(float _pitch) { m_pitch = _pitch; }
	float GetPitch() { return m_pitch; }

	void SetPan(float _pan) { m_pan = _pan; }
	float GetPan() { return m_pan; }

protected:
	std::unique_ptr<DirectX::SoundEffect> m_sfx;

	float m_volume = 1.0f;
	float m_pitch = 0.0f;
	float m_pan = 0.0f;
};

