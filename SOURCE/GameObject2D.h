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

	Vector2 GetPos();
	float GetOri() { return m_orientation; }
	XMVECTORF32 GetColour() { return m_colour; }
	Vector2 GetScale();

	//Set position - if the position should not be scaled adjusted (e.g. working off of current window size), declare "false" in parameter 2.
	void SetPos(Vector2 _pos, bool adjust_for_scale=true);

	void SetOri(float _ori) { m_orientation = _ori; }
	void SetColour(XMVECTORF32 _col) { m_colour = _col; }
	void SetScale(float _scale);
	void SetScale(Vector2 _scale);

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

