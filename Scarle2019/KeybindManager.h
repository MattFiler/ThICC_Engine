#pragma once

#include "GameStateData.h"
#include "GameFilepaths.h"
#include <iostream>
#include <fstream>
#include <json.hpp>
using json = nlohmann::json;

/* User-defined keybinds (and handling for local use) */
struct KeybindManager
{
public:
	KeybindManager() = default;
	~KeybindManager() = default;

	// Initial setup of keybind values
	void setup() {
		//Read in config
		std::ifstream i(m_filepath.generateConfigFilepath("Keybinds", m_filepath.CONFIG));
		config << i;
	};

	// Initialisation of key event handler
	void registerEvent(GameStateData* _GSD) { 
		m_GSD = _GSD;
	}

	// Check if key is currently down
	bool keyDown(const std::string& keybind)
	{
		if (checkKeybind(keybind))
		{
			//return (key_event->key == config[keybind] && key_event->action == ASGE::KEYS::KEY_RELEASED);
			return (m_GSD->m_keyboardState.P);
		}
		return false;
	}

	// Check to see if key was pressed
	bool keyPressed(const std::string& keybind)
	{
		if (checkKeybind(keybind))
		{
			//return (key_event->key == config[keybind] && key_event->action == ASGE::KEYS::KEY_PRESSED);
			return (m_GSD->m_keyboardState.P && !m_GSD->m_prevKeyboardState.P);
		}
		return false;
	}

private:
	bool checkKeybind(const std::string& keybind)
	{
		if (config[keybind].is_null())
		{
			std::cout << "WARNING: ATTEMPTED TO ACCESS UNINITIALISED KEYBIND - " << keybind;
			return false;
		}
		return true;
	}

	static GameStateData* m_GSD;
	static json config;
	GameFilepaths m_filepath;
};