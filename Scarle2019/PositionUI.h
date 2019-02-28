#pragma once

class PositionUI : public GameObject2D
{
public:
	PositionUI(Vector2 _screenPos);
	~PositionUI();

private:
	Text2D *m_positionText = nullptr;
};

