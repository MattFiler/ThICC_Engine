#pragma once
#include "fileVBGO.h"
class TransFileVBGO :
	public FileVBGO
{
public:
	TransFileVBGO(std::string _fileName, RenderData * _RD);
	virtual ~TransFileVBGO();
};

