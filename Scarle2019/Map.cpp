#include "pch.h"
#include "Map.h"
#include <fstream>

MapInstance::MapInstance(const std::string& map_name)
{
	std::ifstream i(m_filepath.generateConfigFilepath("MAP_CONFIG", m_filepath.CONFIG));
	json m_track_data_j;
	m_track_data_j << i;

	if (!m_track_data_j[map_name].is_null())
	{
		m_track_data_j = m_track_data_j[map_name];
	}
	else
	{
		throw std::exception("Requested map does not exist.");
	}

	/*
	map_config.menu_sprite = m_track_data_j["menu_sprite"];
	map_config.friendly_name = m_track_data_j["friendly_name"];
	map_config.model = m_track_data_j["model"];
	map_config.audio_background_start = m_track_data_j["audio"]["background_start"];
	map_config.audio_background = m_track_data_j["audio"]["background"];
	map_config.audio_final_lap_start = m_track_data_j["audio"]["final_lap_start"];
	map_config.audio_final_lap = m_track_data_j["audio"]["final_lap"];
	*/
}

MapData& MapInstance::data()
{
	return map_config;
}
