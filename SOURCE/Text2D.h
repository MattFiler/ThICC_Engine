#pragma once
#include "pch.h"
#include "GameObject2D.h"
#include "GameFilepaths.h"
#include <string>

//GEP:: Draws a line of text to the screen
// DirectXTK12 wiki page: https://github.com/Microsoft/DirectXTK12/wiki/Drawing-text

class Text2D :
	public GameObject2D
{
public:
	Text2D(std::string _text);
	virtual ~Text2D();

	void SetText(std::string _text);
	std::string GetText() { return m_text; }
	XMFLOAT4 GetSize() { return size; }

	//place the origin at the middle of the string
	void CentreOrigin();

	void Render();
	virtual void Reset() override {};

protected:

	std::string m_text;
	std::wstring m_wText;
	bool m_dirtyOrigin = false;

	GameFilepaths m_filepath;
	XMFLOAT4 size;    //the float where we copy the v2 vector members

};

