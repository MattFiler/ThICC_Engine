#pragma once
#include "pch.h"

//GEP:: Base class for all 2-D objects

using namespace DirectX;
using namespace DirectX::SimpleMath;

//Predefinitions
struct ThICC_RenderData;
struct ThICC_GameStateData;

class GameObject2D
{
public:
	GameObject2D();
	virtual ~GameObject2D();

	Vector2 GetPos() { return m_pos; }
	Vector2 GetOrigin() { return m_origin; }
	float GetOri() { return m_orientation; }
	XMVECTORF32 GetColour() { return m_colour; }
	Vector2 GetScale() { return m_scale; }

	void SetPos(Vector2 _pos) { m_pos = _pos; }
	void SetOrigin(Vector2 _origin) { m_origin = _origin; }
	void SetOri(float _ori) { m_orientation = _ori; }
	void SetColour(XMVECTORF32 _col) { m_colour = _col; }
	void SetScale(float _scale) { m_scale = _scale * Vector2::One; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }


	virtual void CentreOrigin() = 0;

	virtual void Tick() {};
	virtual void Render() = 0;

	virtual void Reset() {};

protected:
	Vector2 m_pos = Vector2::Zero;
	Vector2 m_origin = Vector2::Zero;
	float m_orientation = 0.0f;
	XMVECTORF32 m_colour = Colors::White;
	Vector2 m_scale = Vector2::One;
};

