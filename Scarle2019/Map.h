#pragma once
#include "GameFilepaths.h"
#include <json.hpp>
#include <string>
using json = nlohmann::json;

struct MapData
{
	std::string menu_sprite = "";
	std::string friendly_name;
	std::string model;
	std::string audio_background_start;
	std::string audio_background;
	std::string audio_final_lap_start;
	std::string audio_final_lap;
};

class MapInstance {
public:
	MapInstance(const std::string& map_name);
	~MapInstance() = default;

	MapData& data();

private:
	GameFilepaths m_filepath;
	MapData map_config;
};