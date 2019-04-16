#include "pch.h"
#include "light.h"
#include "GameStateData.h"

Light::Light(Vector3 _pos, Color _colour, Color _ambientColour)
{
	m_pos = _pos;
	m_colour = _colour;
	m_ambientColour = _ambientColour;
}

Light::~Light()
{
}

void Light::Tick(GameStateData* _GSD)
{
	//not really needed but spins the light around to show off the lambert default VBGO shader
	static float time = 0.0f;
	time += _GSD->m_dt;
	m_pos.x = 100.0f * cos(time);
	m_pos.z = 100.0f * sin(time);
	if (time > XM_2PI) time -= XM_2PI;
}