#pragma once
#include "VBGO3D.h"

//=================================================================
//create a VBGO using data from a file 
//format is a sort-of a simplified version of the obj format
//=================================================================

class FileVBGO : public VBGO3D
{
public:
	FileVBGO(std::string _fileName);
	virtual ~FileVBGO();

private:

	//data supplied by file tex coordinates or colours
	bool m_texCoords;
	bool m_colour;
};