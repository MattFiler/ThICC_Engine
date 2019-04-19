#pragma once
#include "pch.h"
#include "ImageGO2D.h"
#include "LocalisationManager.h"
#include <json.hpp>
using json = nlohmann::json;

//A struct to store map info like our scene index, localised name, audio and preview sprite
struct MapInfo {
	MapInfo() {};
	MapInfo(json _element, int _index) {
		LocalisationManager m_localiser;

		name = m_localiser.getString(_element["friendly_name"]);
		model = _element["model"];
		preview_sprite = new ImageGO2D(_element["menu_sprite"]);
		scene_index = _index;

		audio_background_start = _element["audio"]["background_start"];
		audio_background = _element["audio"]["background"];
		audio_final_lap_start = _element["audio"]["final_lap_start"];
		audio_final_lap = _element["audio"]["final_lap"];
	};

	ImageGO2D* preview_sprite = nullptr;
	std::string name = "";
	std::string model = "";
	int scene_index = -1;

	std::string audio_background_start = "";
	std::string audio_background = "";
	std::string audio_final_lap_start = "";
	std::string audio_final_lap = "";
};