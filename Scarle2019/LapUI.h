#pragma once

class LapUI : public GameObject2D
{
public:
	LapUI(Vector2 _screenPos);
	~LapUI();

private:
	Text2D *m_lapText = nullptr;
};

