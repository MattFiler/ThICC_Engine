#pragma once
#include "pch.h"
#include "ImageGO2D.h"
#include "LocalisationManager.h"
#include "GameFilepaths.h"
#include "AssetComp.h"
#include "GameObjectShared.h"
#include "CupInfo.h"
#include <json.hpp>
#include <codecvt>
using json = nlohmann::json;

//A struct to store map info like our scene index, localised name, audio and preview sprite
struct MapInfo : public AssetComp {
	MapInfo() {};
	MapInfo(json _element, int _index) {
		LocalisationManager m_localiser;
		GameFilepaths m_filepaths;
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

		name = m_localiser.getString(_element["friendly_name"]);
		model = _element["model"];
		decoration_model = _element["decoration_model"];
		preview_sprite = new ImageGO2D(_element["menu_sprite"]);
		scene_index = _index;

		is_arcade_exclusive = _element["arcade_only"];

		for (CupInfo* this_cup : Locator::getGOS()->cup_instances) {
			if (this_cup->codename == _element["cup"]) {
				cup = this_cup;
			}
		}
			
		std::string cubemap_radiance_str = m_filepaths.generateFilepath(_element["cubemap"], m_filepaths.CUBEMAP_RADIANCE);
		cubemap_radiance = converter.from_bytes(cubemap_radiance_str.c_str());
		std::string cubemap_irradiance_str = m_filepaths.generateFilepath(_element["cubemap"], m_filepaths.CUBEMAP_IRRADIANCE);
		cubemap_irradiance = converter.from_bytes(cubemap_irradiance_str.c_str());
		std::string cubemap_skybox_str = m_filepaths.generateFilepath(_element["skybox"], m_filepaths.IMAGE);
		cubemap_skybox = converter.from_bytes(cubemap_skybox_str.c_str());

		audio_background_start = _element["audio"]["background_start"];
		audio_background = _element["audio"]["background"];
		audio_final_lap_start = _element["audio"]["final_lap_start"];
		audio_final_lap = _element["audio"]["final_lap"];
	};

	ImageGO2D* preview_sprite = nullptr;
	std::string name = "";
	std::string model = "";
	std::string decoration_model = "";
	CupInfo* cup = nullptr;
	int scene_index = -1;

	bool is_arcade_exclusive = false;

	std::wstring cubemap_radiance;
	std::wstring cubemap_irradiance;
	std::wstring cubemap_skybox;

	std::string audio_background_start = "";
	std::string audio_background = "";
	std::string audio_final_lap_start = "";
	std::string audio_final_lap = "";
};