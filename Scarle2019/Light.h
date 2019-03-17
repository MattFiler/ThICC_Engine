#pragma once
#include "GameObject3D.h"
#include "ServiceLocator.h"

//=================================================================
//a simple light defined by a colour and and ambient colour and 
//used as a point light with no drop off in the standard shader 
//used to draw VBGOs
//=================================================================

struct DrawData;

class Light : public GameObject3D
{
public:
	Light(Vector3 _pos, Color _colour, Color _ambientColour);
	~Light();

	virtual void Render() override { Locator::getRD(); }; //draws nothing

	virtual void Tick() override;

	//getters
	Color GetColour() { return m_colour; }
	Color GetAmbCol() { return m_ambientColour; }

	//setters
	void SetColor(Color _colour) { m_colour = _colour; }
	void SetAmbCol(Color _colour) { m_ambientColour = _colour; }

protected:

	Color m_colour;
	Color m_ambientColour;
};