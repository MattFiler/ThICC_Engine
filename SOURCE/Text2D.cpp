#include "pch.h"
#include "Text2D.h"
#include "renderdata.h"
#include <codecvt>


Text2D::Text2D(std::string _text)
{
	SetText(_text);
}

void Text2D::SetText(std::string _text)
{
	m_text = _text;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	m_wText = converter.from_bytes(m_text.c_str());
	XMVECTOR v2 = Locator::getRD()->m_font->MeasureString(m_wText.c_str());
	XMStoreFloat4(&size, v2);
}

Text2D::~Text2D()
{
}

void Text2D::CentreOrigin()
{
	m_dirtyOrigin = true;
}

void Text2D::Render()
{
	if (m_dirtyOrigin)
	{
		//needs to be done here as requires the font being used
		//so made  this hack
		m_dirtyOrigin = false;
		m_origin = Locator::getRD()->m_font->MeasureString(m_wText.c_str()) / 2.f;
	}

	Locator::getRD()->m_font->DrawString(Locator::getRD()->m_spriteBatch.get(), m_wText.c_str(), m_pos, m_colour, m_orientation, m_scale);
}
