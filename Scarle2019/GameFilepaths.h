#pragma once
#include <string>

// Filepaths to our assets - these may change during production,
// so nicer to reference from somewhere easier to modify.
struct GameFilepaths {
public:
	enum m_asset_type { SHADER, MODEL, MODEL_CUSTOM, MODEL_COLLMAP, IMAGE, SOUND };
	std::string generateFilepath(std::string _asset_name, m_asset_type _asset_type) {
		switch (_asset_type) {
		case SHADER:
			return shader_path[FOLDER] + _asset_name + shader_path[EXTENSION];
		case MODEL:
			return model_path[FOLDER] + _asset_name + "/" + _asset_name + model_path[EXTENSION];
		case MODEL_CUSTOM:
			return model_custom_path[FOLDER] +_asset_name + "/" + _asset_name + model_custom_path[EXTENSION];
		case MODEL_COLLMAP:
			return model_collmap_path[FOLDER] + _asset_name + "/" + _asset_name + model_collmap_path[EXTENSION];
		case IMAGE:
			return image_path[FOLDER] + _asset_name + image_path[EXTENSION];
		case SOUND:
			return sound_path[FOLDER] + _asset_name + sound_path[EXTENSION];
		}
	}
	std::string getFolder(m_asset_type _asset_type) {
		return getPathSection(_asset_type, FOLDER);
	}
	std::string getExtension(m_asset_type _asset_type) {
		return getPathSection(_asset_type, EXTENSION);
	}
private:
	enum m_path_data { FOLDER, EXTENSION };

	std::string shader_path[2] = { "../DATA/SHADERS/", ".hlsl" };
	std::string model_path[2] = { "../DATA/MODELS/", ".sdkmesh" };
	std::string model_custom_path[2] = { "../DATA/MODELS/", ".txt" };
	std::string model_collmap_path[2] = { "../DATA/MODELS/", ".collmap" };
	std::string image_path[2] = { "../DATA/IMAGES/", ".dds" };
	std::string sound_path[2] = { "../DATA/SOUNDS/", ".wav" };

	std::string getPathSection(m_asset_type _asset_type, m_path_data _path_section) {
		switch (_asset_type) {
		case SHADER:
			return shader_path[_path_section];
		case MODEL:
			return model_path[_path_section];
		case MODEL_CUSTOM:
			return model_custom_path[_path_section];
		case MODEL_COLLMAP:
			return model_collmap_path[_path_section];
		case IMAGE:
			return image_path[_path_section];
		case SOUND:
			return sound_path[_path_section];
		}
	}
};