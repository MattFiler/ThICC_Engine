#pragma once
#include "pch.h"
#include "ImageGO2D.h"
#include "LocalisationManager.h"
#include "AssetComp.h"
#include <json.hpp>
using json = nlohmann::json;

//A struct to store cup info like name and sprite
struct CupInfo : public AssetComp {
	CupInfo() {};
	CupInfo(std::string _key, json _element) {
		LocalisationManager m_localiser;

		name = m_localiser.getString(_element["friendly_name"]);
		preview_sprite = new ImageGO2D(_element["menu_sprite"]);

		codename = _key;
	};

	ImageGO2D* preview_sprite = nullptr;
	std::string name = "";

	std::string codename = "";
};