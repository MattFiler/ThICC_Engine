#pragma once
#include "Sound.h"

//GEP:: An example sopund which goes off when you press space.

class TestSound 
{
public:
	TestSound(AudioEngine* _audEngine, string _filename, bool _loop);
	~TestSound();

	virtual void Tick(GameStateData* _GSD);
};

