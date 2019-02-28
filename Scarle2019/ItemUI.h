#pragma once


class ItemUI : public GameObject2D
{
public:
	ItemUI(Vector2 screenPos);
	~ItemUI();

private:
	ImageGO2D *m_activeItemSprite = nullptr;
};

