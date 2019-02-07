#include "pch.h"
#include "TestSound.h"
#include "GameStateData.h"


TestSound::TestSound(AudioEngine* _audEngine, string _filename) :Sound(_audEngine, _filename)
{
}


TestSound::~TestSound()
{
}

void TestSound::Tick(GameStateData * _GSD)
{
	if (_GSD->m_keyboardState.Space && !_GSD->m_prevKeyboardState.Space)
	{
		Play();
	}
}
