#pragma once

#include "InputData.h"
#include "GameFilepaths.h"
#include "GameStateData.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <json.hpp>
using json = nlohmann::json;
using DX_BUTTON_STATE = DirectX::GamePad::ButtonStateTracker::ButtonState;

/* User-defined keybinds (and handling for local use) */
struct KeybindManager
{
public:
	KeybindManager() = default;
	~KeybindManager() = default;

	// Initial setup of keybind values
	void setup(ThICC_InputData* _GSD) {
		//Read in config
		std::ifstream i(m_filepath.generateConfigFilepath("Keybinds_Config", m_filepath.CONFIG));
		config << i;

		//Save static pointer to keybind event data
		m_ID = _GSD;
	};

	// Check to see if key was released
	bool keyReleased(const std::string& keybind)
	{
		//Format keybind
		std::string this_keybind = formatKeybind(keybind);

		if (checkKeybind(this_keybind))
		{
			#ifdef _ARCADE
			//Check arcade
			if (config[this_keybind]["Arcade"] != "DISABLED") {
				if (m_ID->m_keyboardTracker.IsKeyReleased((DirectX::Keyboard::Keys)config[this_keybind]["Arcade_Code"])) {
					return true;
				}
			}
			#else
			//Check keyboard
			if (config[this_keybind]["Keyboard"] != "DISABLED") {
				if (m_ID->m_keyboardTracker.IsKeyReleased((DirectX::Keyboard::Keys)config[this_keybind]["Keyboard_Code"])) {
					return true;
				}
			}

			//Check gamepads
			if (config[this_keybind]["Gamepad"] != "DISABLED") {
				for (int i = 0; i < 4; i++) {
					if (config[this_keybind]["Gamepad"] == "A") { if (m_ID->m_gamepadButtonTracker[i].a == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "B") { if (m_ID->m_gamepadButtonTracker[i].b == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "X") { if (m_ID->m_gamepadButtonTracker[i].x == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Y") { if (m_ID->m_gamepadButtonTracker[i].y == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick") { if (m_ID->m_gamepadButtonTracker[i].leftStick == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick") { if (m_ID->m_gamepadButtonTracker[i].rightStick == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Shoulder") { if (m_ID->m_gamepadButtonTracker[i].leftShoulder == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Shoulder") { if (m_ID->m_gamepadButtonTracker[i].rightShoulder == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Back") { if (m_ID->m_gamepadButtonTracker[i].back == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Start") { if (m_ID->m_gamepadButtonTracker[i].start == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "D-Pad Up") { if (m_ID->m_gamepadButtonTracker[i].dpadUp == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "D-Pad Down") { if (m_ID->m_gamepadButtonTracker[i].dpadDown == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "D-Pad Left") { if (m_ID->m_gamepadButtonTracker[i].dpadLeft == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "D-Pad Right") { if (m_ID->m_gamepadButtonTracker[i].dpadRight == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick Up") { if (m_ID->m_gamepadButtonTracker[i].leftStickUp == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick Down") { if (m_ID->m_gamepadButtonTracker[i].leftStickDown == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick Left") { if (m_ID->m_gamepadButtonTracker[i].leftStickLeft == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick Right") { if (m_ID->m_gamepadButtonTracker[i].leftStickRight == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick Up") { if (m_ID->m_gamepadButtonTracker[i].rightStickUp == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick Down") { if (m_ID->m_gamepadButtonTracker[i].rightStickDown == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick Left") { if (m_ID->m_gamepadButtonTracker[i].rightStickLeft == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick Right") { if (m_ID->m_gamepadButtonTracker[i].rightStickRight == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Trigger") { if (m_ID->m_gamepadButtonTracker[i].leftTrigger == DX_BUTTON_STATE::RELEASED) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Trigger") { if (m_ID->m_gamepadButtonTracker[i].rightTrigger == DX_BUTTON_STATE::RELEASED) { return true; } }
				}
			}
			#endif
		}
		return false;
	}

	// Check to see if key is being held
	bool keyHeld(const std::string& keybind)
	{
		//Format keybind
		std::string this_keybind = formatKeybind(keybind);

		if (checkKeybind(this_keybind))
		{
			#ifdef _ARCADE
			//Check arcade
			if (config[this_keybind]["Arcade"] != "DISABLED") {
				if (m_ID->m_keyboardTracker.IsKeyPressed((DirectX::Keyboard::Keys)config[this_keybind]["Arcade_Code"])) {
					return true;
				}
			}
			#else
			//Check keyboard
			if (config[this_keybind]["Keyboard"] != "DISABLED") {
				if (m_ID->m_keyboardTracker.IsKeyPressed((DirectX::Keyboard::Keys)config[this_keybind]["Keyboard_Code"])) {
					return true;
				}
			}

			//Check gamepads
			if (config[this_keybind]["Gamepad"] != "DISABLED") {
				for (int i = 0; i < 4; i++) {
					if (config[this_keybind]["Gamepad"] == "A") { if (m_ID->m_gamepadButtonTracker[i].a == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "B") { if (m_ID->m_gamepadButtonTracker[i].b == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "X") { if (m_ID->m_gamepadButtonTracker[i].x == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Y") { if (m_ID->m_gamepadButtonTracker[i].y == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick") { if (m_ID->m_gamepadButtonTracker[i].leftStick == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick") { if (m_ID->m_gamepadButtonTracker[i].rightStick == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Shoulder") { if (m_ID->m_gamepadButtonTracker[i].leftShoulder == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Shoulder") { if (m_ID->m_gamepadButtonTracker[i].rightShoulder == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Back") { if (m_ID->m_gamepadButtonTracker[i].back == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Start") { if (m_ID->m_gamepadButtonTracker[i].start == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "D-Pad Up") { if (m_ID->m_gamepadButtonTracker[i].dpadUp == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "D-Pad Down") { if (m_ID->m_gamepadButtonTracker[i].dpadDown == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "D-Pad Left") { if (m_ID->m_gamepadButtonTracker[i].dpadLeft == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "D-Pad Right") { if (m_ID->m_gamepadButtonTracker[i].dpadRight == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick Up") { if (m_ID->m_gamepadButtonTracker[i].leftStickUp == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick Down") { if (m_ID->m_gamepadButtonTracker[i].leftStickDown == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick Left") { if (m_ID->m_gamepadButtonTracker[i].leftStickLeft == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Stick Right") { if (m_ID->m_gamepadButtonTracker[i].leftStickRight == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick Up") { if (m_ID->m_gamepadButtonTracker[i].rightStickUp == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick Down") { if (m_ID->m_gamepadButtonTracker[i].rightStickDown == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick Left") { if (m_ID->m_gamepadButtonTracker[i].rightStickLeft == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Stick Right") { if (m_ID->m_gamepadButtonTracker[i].rightStickRight == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Left Trigger") { if (m_ID->m_gamepadButtonTracker[i].leftTrigger == DX_BUTTON_STATE::HELD) { return true; } }
					if (config[this_keybind]["Gamepad"] == "Right Trigger") { if (m_ID->m_gamepadButtonTracker[i].rightTrigger == DX_BUTTON_STATE::HELD) { return true; } }
				}
			}
			#endif
		}
		return false;
	}

private:
	/* Check keybind validity*/
	bool checkKeybind(const std::string& keybind)
	{
		//Ignore first 5 frames because DirectX inputs are brilliant.
		if (Locator::getGSD()->m_timer.GetFrameCount() < 5)
		{
			return false; 
		}

		//Make sure keybind exists
		if (config[keybind].is_null())
		{
			DebugText::print("Attempted to access an undeclared keybind: #" + keybind);
			return false;
		}

		//If this is a debug keybind, disable it in release
		#ifndef _DEBUG
		if (config[keybind]["is_debug"]) 
		{
			return false;
		}
		#endif

		//Keybind is valid
		return true;
	}

	/* Format keybind to the standard */
	std::string formatKeybind(const std::string& keybind) {
		std::string this_keybind = keybind;
		std::transform(this_keybind.begin(), this_keybind.end(), this_keybind.begin(), ::toupper);
		for (unsigned int i = 0; i < this_keybind.size(); i++) {
			if (this_keybind[i] == ' ') {
				this_keybind[i] = '_';
			}
		}
		return this_keybind;
	}

	static ThICC_InputData* m_ID;
	static json config;
	GameFilepaths m_filepath;
};