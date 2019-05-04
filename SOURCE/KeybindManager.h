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

	/* Initial setup of keybind values */
	void setup(ThICC_InputData* _GSD) {
		//Read in config
		std::ifstream i(m_filepath.generateConfigFilepath("Keybinds_Config", m_filepath.CONFIG));
		config << i;

		//Save static pointer to keybind event data
		m_ID = _GSD;
	};

	/* Reset internal trackers */
	void Reset() {
		for (int i = 0; i < 254; i++) {
			key_held[i] = false;
		}
	}

	/* Check to see if key was released */
	bool keyReleased(const std::string& keybind, int gamepad = -1)
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

			//Check gamepad(s)
			if (config[this_keybind]["Gamepad"] != "DISABLED") {
				if (gamepad == -1) {
					for (int i = 0; i < 4; i++) {
						if (gamepadButton(this_keybind, i, DX_BUTTON_STATE::RELEASED)) {
							return true;
						}
					}
				}
				else
				{
					if (gamepadButton(this_keybind, gamepad, DX_BUTTON_STATE::RELEASED)) {
						return true;
					}
				}
			}
			#endif
		}
		return false;
	}

	/* Check to see if key is being held */
	bool keyHeld(const std::string& keybind, int gamepad = -1)
	{
		//Format keybind
		std::string this_keybind = formatKeybind(keybind);

		if (checkKeybind(this_keybind))
		{
			#ifdef _ARCADE
			//Check arcade
			if (config[this_keybind]["Arcade"] != "DISABLED") {
				if (m_ID->m_keyboardTracker.IsKeyPressed((DirectX::Keyboard::Keys)config[this_keybind]["Arcade_Code"])) {
					key_held[config[this_keybind]["Arcade_Code"]] = true;
				}
				if (m_ID->m_keyboardTracker.IsKeyReleased((DirectX::Keyboard::Keys)config[this_keybind]["Arcade_Code"])) {
					key_held[config[this_keybind]["Arcade_Code"]] = false;
				}
				if (key_held[config[this_keybind]["Arcade_Code"]]) {
					return true;
				}
			}
			#else
			//Check keyboard
			if (config[this_keybind]["Keyboard"] != "DISABLED") {
				if (m_ID->m_keyboardTracker.IsKeyPressed((DirectX::Keyboard::Keys)config[this_keybind]["Keyboard_Code"])) {
					key_held[config[this_keybind]["Keyboard_Code"]] = true;
				}
				if (m_ID->m_keyboardTracker.IsKeyReleased((DirectX::Keyboard::Keys)config[this_keybind]["Keyboard_Code"])) {
					key_held[config[this_keybind]["Keyboard_Code"]] = false;
				}
				if (key_held[config[this_keybind]["Keyboard_Code"]]) {
					return true;
				}
			}

			//Check gamepad(s)
			if (config[this_keybind]["Gamepad"] != "DISABLED") {
				if (gamepad == -1) {
					for (int i = 0; i < 4; i++) {
						if (gamepadButton(this_keybind, i, DX_BUTTON_STATE::HELD)) {
							return true;
						}
					}
				}
				else
				{
					if (gamepadButton(this_keybind, gamepad, DX_BUTTON_STATE::HELD)) {
						return true;
					}
				}
			}
			#endif
		}
		return false;
	}

private:
	/* Check keybind validity */
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

	/* Gamepad button held */
	bool gamepadButton(const std::string& this_keybind, int i, DX_BUTTON_STATE button_state) {
		if (!m_ID->m_gamepad->GetState(i).connected) { return false; }
		if (config[this_keybind]["Gamepad"] == "A") { if (m_ID->m_gamepadButtonTracker[i].a == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "B") { if (m_ID->m_gamepadButtonTracker[i].b == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "X") { if (m_ID->m_gamepadButtonTracker[i].x == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Y") { if (m_ID->m_gamepadButtonTracker[i].y == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Left Stick") { if (m_ID->m_gamepadButtonTracker[i].leftStick == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Right Stick") { if (m_ID->m_gamepadButtonTracker[i].rightStick == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Left Shoulder") { if (m_ID->m_gamepadButtonTracker[i].leftShoulder == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Right Shoulder") { if (m_ID->m_gamepadButtonTracker[i].rightShoulder == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Back") { if (m_ID->m_gamepadButtonTracker[i].back == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Start") { if (m_ID->m_gamepadButtonTracker[i].start == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "D-Pad Up") { if (m_ID->m_gamepadButtonTracker[i].dpadUp == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "D-Pad Down") { if (m_ID->m_gamepadButtonTracker[i].dpadDown == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "D-Pad Left") { if (m_ID->m_gamepadButtonTracker[i].dpadLeft == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "D-Pad Right") { if (m_ID->m_gamepadButtonTracker[i].dpadRight == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Left Stick Up") { if (m_ID->m_gamepadButtonTracker[i].leftStickUp == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Left Stick Down") { if (m_ID->m_gamepadButtonTracker[i].leftStickDown == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Left Stick Left") { if (m_ID->m_gamepadButtonTracker[i].leftStickLeft == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Left Stick Right") { if (m_ID->m_gamepadButtonTracker[i].leftStickRight == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Right Stick Up") { if (m_ID->m_gamepadButtonTracker[i].rightStickUp == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Right Stick Down") { if (m_ID->m_gamepadButtonTracker[i].rightStickDown == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Right Stick Left") { if (m_ID->m_gamepadButtonTracker[i].rightStickLeft == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Right Stick Right") { if (m_ID->m_gamepadButtonTracker[i].rightStickRight == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Left Trigger") { if (m_ID->m_gamepadButtonTracker[i].leftTrigger == button_state) { return true; } }
		if (config[this_keybind]["Gamepad"] == "Right Trigger") { if (m_ID->m_gamepadButtonTracker[i].rightTrigger == button_state) { return true; } }
	}

	static ThICC_InputData* m_ID;
	static json config;
	GameFilepaths m_filepath;
	bool key_held[254] = { 0 };
};