#pragma once
#include "GameFilepaths.h"
#include "SimpleMath.h"
#include "Constants.h"
#include <json.hpp>
using json = nlohmann::json;
using Vector2 = DirectX::SimpleMath::Vector2;

class ImageGO2D;
class Text2D;
class Player;

class InGameUI
{
public:
	InGameUI(Vector2 size, Vector2 offset);
	~InGameUI();

	void ExpensiveLoad();
	void ExpensiveUnload();

	void SetState(InGameInterfaceState state);

	void SetMapName(const std::string& name);
	void SetCurrentLap(int lap);
	void SetPlayerPosition(int position);
	void ShowItemSpinner(Player* this_player);
	void HideItemSpinner(); 

	void Render();
	void Update();

private:
	/* Sprites to queue (coming up) */
	std::vector<ImageGO2D*> lap_ui_sprites;
	std::vector<ImageGO2D*> position_ui_sprites;

	/* Sprites to display (current) */
	ImageGO2D* intro_ui_sprite = nullptr;
	ImageGO2D* lap_ui_sprite = nullptr;
	ImageGO2D* position_ui_sprite = nullptr;
	ImageGO2D* item_ui_sprite = nullptr;
	ImageGO2D* outro_ui_sprite = nullptr;

	/* Text to display */
	Text2D* intro_ui_text = nullptr;

	/* Engine functionality */
	GameFilepaths m_filepath;

	/* Configuration */
	json config;

	/* States & info */
	InGameInterfaceState current_state = InGameInterfaceState::UI_HIDDEN;
	Vector2 ui_offset = Vector2(0,0);
	Vector2 ui_size = Vector2(0, 0);
};

