#include "pch.h"
#include "Text2D.h"
#include <codecvt>

/* Create */
Text2D::Text2D(std::string _text, TextOrigin _origin)
{
	SetText(_text);

	m_originPos = _origin;

	if (middle_origin) {
		//Start in centre as default
		m_pos.x = Locator::getRD()->m_window_width / 2.f;
		m_pos.y = Locator::getRD()->m_window_height / 2.f;
	}
}

/* Set the text to render */
void Text2D::SetText(std::string _text)
{
	m_text = _text;
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	m_wText = converter.from_bytes(m_text.c_str());
}

/* Render text */
void Text2D::Render()
{
	switch (m_originPos) {
	case TextOrigin::BOTTOM_LEFT:
		m_origin = Vector2(0, ((Vector2)Locator::getRD()->m_2dFont->MeasureString(m_wText.c_str())).y);
		break;
	case TextOrigin::BOTTOM_RIGHT:
		m_origin = Locator::getRD()->m_2dFont->MeasureString(m_wText.c_str());
		break;
	case TextOrigin::MIDDLE:
		m_origin = Locator::getRD()->m_2dFont->MeasureString(m_wText.c_str()) / 2.f;
		break;
	case TextOrigin::TOP_RIGHT:
		m_origin = Vector2(((Vector2)Locator::getRD()->m_2dFont->MeasureString(m_wText.c_str())).x, 0);
		break;
	}

	//Render
	Locator::getRD()->m_2dFont->DrawString(Locator::getRD()->m_2dSpriteBatch.get(), m_wText.c_str(),
		m_pos, m_colour, 0.f, m_origin, m_scale);
}
