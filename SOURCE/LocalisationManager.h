#pragma once

#include "pch.h"
#include "GameFilepaths.h"
#include <iostream>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

struct LocalisationManager
{
	void configure(const std::string& language)
	{
		language_definition = language;
		std::ifstream i(m_filepath.generateConfigFilepath("Localisation", m_filepath.CONFIG));
		language_config << i;
		language_config = language_config[language];
	}
	std::string getString(const std::string& id)
	{
		if (language_config[id].is_string())
		{
			return language_config[id];
		}
		//Instead of a throw, fail nicely by logging and printing the requested string id.
		DebugText::print("COULDN'T FIND A LOCALISATION FOR '#" + id + "'");
		return "#" + id;
	}
	std::string getLanguage() {
		return language_definition;
	}

private:
	static json language_config;
	static std::string language_definition;
	GameFilepaths m_filepath;
};