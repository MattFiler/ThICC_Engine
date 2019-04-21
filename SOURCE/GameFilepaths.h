#pragma once
#include <algorithm>
#include <string>

// Filepaths to our assets - these may change during production,
// so nicer to reference from somewhere easier to modify.
struct GameFilepaths {
public:
	enum m_asset_type { 
		SHADER, 
		MODEL, 
		MODEL_CUSTOM, 
		MODEL_COLLMAP, 
		IMAGE, 
		SOUND, 
		FONT, 
		CONFIG, 
		CUBEMAP_SPECULAR, // AKA Irradiance
		CUBEMAP_DIFFUSE   // AKA Radiance
	};

	/* Generate filepath */
	std::string generateFilepath(std::string _asset_name, m_asset_type _asset_type) {
		//Convert to uppercase
		std::transform(_asset_name.begin(), _asset_name.end(), _asset_name.begin(), ::toupper); 
		
		//Replace all spaces in filename with underscores for models
		if (_asset_type == MODEL) {
			char space = ' ';
			for (unsigned int i = 0; i < _asset_name.size(); i++) {
				if (_asset_name[i] == ' ') {
					_asset_name[i] = '_';
				}
			}
		}

		//Put together filepath and return
		switch (_asset_type) {
		case SHADER:
			return shader_path[FOLDER] + _asset_name + shader_path[EXTENSION];
		case MODEL:
			if (_asset_name.length() > 6 && _asset_name.substr(_asset_name.length() - 5) == "DEBUG") {
				//Fix the path for debug meshes
				std::string original_string = _asset_name.substr(0, _asset_name.length() - 6);
				return model_path[FOLDER] + original_string + "/" + original_string + " DEBUG" + model_path[EXTENSION];
			}
			return model_path[FOLDER] + _asset_name + "/" + _asset_name + model_path[EXTENSION];
		case MODEL_CUSTOM:
			return model_custom_path[FOLDER] +_asset_name + "/" + _asset_name + model_custom_path[EXTENSION];
		case MODEL_COLLMAP:
			return model_collmap_path[FOLDER] + _asset_name + "/" + _asset_name + model_collmap_path[EXTENSION];
		case IMAGE:
			return image_path[FOLDER] + _asset_name + image_path[EXTENSION];
		case SOUND:
			return sound_path[FOLDER] + _asset_name + sound_path[EXTENSION];
		case FONT:
			return font_path[FOLDER] + _asset_name + font_path[EXTENSION];
		case CONFIG:
			return config_path[FOLDER] + _asset_name + config_path[EXTENSION];
		case CUBEMAP_SPECULAR:
			return cubemap_path_s[FOLDER] + _asset_name + cubemap_path_s[EXTENSION];
		case CUBEMAP_DIFFUSE:
			return cubemap_path_d[FOLDER] + _asset_name + cubemap_path_d[EXTENSION];
		}
		return "";
	}
	
	/* Generate filepath for config */
	std::string generateConfigFilepath(std::string _asset_name, m_asset_type _asset_type) {
		std::string config_filepath = generateFilepath(_asset_name, _asset_type);
		return config_filepath.substr(0, config_filepath.length() - getExtension(_asset_type).length()) + ".JSON";
	}

	/* Get a folder path */
	std::string getFolder(m_asset_type _asset_type) {
		return getPathSection(_asset_type, FOLDER);
	}

	/* Get a file extension */
	std::string getExtension(m_asset_type _asset_type) {
		return getPathSection(_asset_type, EXTENSION);
	}
private:
	enum m_path_data { FOLDER, EXTENSION };

	//Paths and extensions
	std::string shader_path[2] = { "DATA/SHADERS/", ".HLSL" };
	std::string model_path[2] = { "DATA/MODELS/", ".SDKMESH" };
	std::string model_custom_path[2] = { "DATA/MODELS/", ".TXTMESH" };
	std::string model_collmap_path[2] = { "DATA/MODELS/", ".COLLMAP" };
	std::string image_path[2] = { "DATA/IMAGES/", ".DDS" };
	std::string sound_path[2] = { "DATA/SOUNDS/", ".WAV" };
	std::string font_path[2] = { "DATA/FONTS/", ".SPRITEFONT" };
	std::string config_path[2] = { "DATA/CONFIGS/", ".JSON" };
	std::string cubemap_path_s[2] = { "DATA/CUBEMAPS/", "_SPECULAR.DDS" };
	std::string cubemap_path_d[2] = { "DATA/CUBEMAPS/", "_DIFFUSE.DDS" };

	/* Internally get our path/extension info for use further */
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
		case FONT:
			return font_path[_path_section];
		case CONFIG:
			return config_path[_path_section];
		case CUBEMAP_SPECULAR:
			return cubemap_path_s[_path_section];
		case CUBEMAP_DIFFUSE:
			return cubemap_path_d[_path_section];
		}
	}
};