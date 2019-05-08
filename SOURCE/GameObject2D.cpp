#include "pch.h"
#include "GameObject2D.h"
#include "RenderData.h"


GameObject2D::GameObject2D()
{
	SetScale(1.0); //triggers a res-scale
}


GameObject2D::~GameObject2D()
{
}

Vector2 GameObject2D::GetPos()
{
	return m_pos / Locator::getRD()->m_base_res_scale;
}

Vector2 GameObject2D::GetScale()
{
	return m_scale / Locator::getRD()->m_base_res_scale;
}

void GameObject2D::SetPos(Vector2 _pos, bool adjust_for_scale)
{
	if (adjust_for_scale) {
		m_pos = _pos * Locator::getRD()->m_base_res_scale;
	}
	else
	{
		m_pos = _pos;
	}
}

void GameObject2D::SetScale(float _scale)
{
	m_scale = (_scale * Vector2::One) * Locator::getRD()->m_base_res_scale;
}

void GameObject2D::SetScale(Vector2 _scale)
{
	m_scale = _scale * Locator::getRD()->m_base_res_scale;
}
