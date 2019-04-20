#pragma once
#include "pch.h"
#include "ImageGO2D.h"
#include "LocalisationManager.h"
#include "AssetComp.h"
#include <json.hpp>
using json = nlohmann::json;

//A struct to store vehicle info like name, sprite, model and stats
struct VehicleInfo : public AssetComp {
	VehicleInfo() {};
	VehicleInfo(json _element) {
		LocalisationManager m_localiser;

		name = m_localiser.getString(_element["friendly_name"]);
		model = _element["model"];
		preview_sprite = new ImageGO2D(_element["menu_sprite"]);
	};

	ImageGO2D* preview_sprite = nullptr;
	std::string name = "";
	std::string model = "";
};