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
		std::transform(language_definition.begin(), language_definition.end(), language_definition.begin(), ::toupper);

		std::ifstream i(m_filepath.generateConfigFilepath("Localisation", m_filepath.CONFIG));
		language_config << i;
		language_config = language_config[language_definition];
	}
	std::string getString(const std::string& id)
	{
		std::string this_id = id;
		std::transform(this_id.begin(), this_id.end(), this_id.begin(), ::toupper);
		if (language_config[this_id].is_string())
		{
			return language_config[this_id];
		}
		//Instead of a throw, fail nicely by logging and printing the requested string id.
		DebugText::print("COULDN'T FIND A LOCALISATION FOR '#" + this_id + "'");
		return "#" + this_id;
	}
	std::string getLanguage() {
		return language_definition;
	}

private:
	static json language_config;
	static std::string language_definition;
	GameFilepaths m_filepath;
};