#pragma once

//=================================================================
//create a snail shell from a base line shape read in from a txt file
//=================================================================

#include "VBGO3D.h"
#include <string>

struct GameData;

class VBSnail : public VBGO3D
{
public:
	VBSnail(std::string _filename, int _sections, float _scale, float _rot, float _step, Color _col1, Color _col2 );
	virtual ~VBSnail(){};

	virtual void Tick() override;

protected:
};