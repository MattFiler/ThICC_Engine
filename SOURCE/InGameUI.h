#pragma once
#include "GameFilepaths.h"
#include <json.hpp>
using json = nlohmann::json;

class ImageGO2D;

class InGameUI
{
public:
	InGameUI();
	~InGameUI();

	void ExpensiveLoad();
	void ExpensiveUnload();

	void Show() { visible = true; }
	void Hide() { visible = false; }

	void SetCurrentLap(int lap);
	void SetPlayerPosition(int position);
	void ShowItemSpinner();
	void HideItemSpinner(); 

	void Render();
	void Update();

private:
	/* Sprites to queue (coming up) */
	std::vector<ImageGO2D*> lap_ui_sprites;
	std::vector<ImageGO2D*> position_ui_sprites;

	/* Sprites to display (current) */
	ImageGO2D* lap_ui_sprite = nullptr;
	ImageGO2D* position_ui_sprite = nullptr;
	ImageGO2D* item_ui_sprite = nullptr; //Never modified, set directly.

	/* Engine functionality */
	GameFilepaths m_filepath;

	/* Configuration */
	json config;

	/* States & info */
	bool visible = false;
};

