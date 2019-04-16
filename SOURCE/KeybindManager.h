#pragma once

#include "InputData.h"
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
	void setup(ThICC_InputData* _GSD) {
		//Read in config
		std::ifstream i(m_filepath.generateConfigFilepath("Keybinds", m_filepath.CONFIG));
		config << i;

		//Save static pointer to keybind event data
		m_ID = _GSD;
	};

	// Check to see if key was pressed
	bool keyPressed(const std::string& keybind)
	{
		if (checkKeybind(keybind))
		{
			if (config[keybind] == "Back") { return (m_ID->m_keyboardState.Back && !m_ID->m_prevKeyboardState.Back); }
			if (config[keybind] == "Tab") { return (m_ID->m_keyboardState.Tab && !m_ID->m_prevKeyboardState.Tab); }
			if (config[keybind] == "Enter") { return (m_ID->m_keyboardState.Enter && !m_ID->m_prevKeyboardState.Enter); }
			if (config[keybind] == "Pause") { return (m_ID->m_keyboardState.Pause && !m_ID->m_prevKeyboardState.Pause); }
			if (config[keybind] == "CapsLock") { return (m_ID->m_keyboardState.CapsLock && !m_ID->m_prevKeyboardState.CapsLock); }
			if (config[keybind] == "Kana") { return (m_ID->m_keyboardState.Kana && !m_ID->m_prevKeyboardState.Kana); }
			if (config[keybind] == "Kanji") { return (m_ID->m_keyboardState.Kanji && !m_ID->m_prevKeyboardState.Kanji); }
			if (config[keybind] == "Escape") { return (m_ID->m_keyboardState.Escape && !m_ID->m_prevKeyboardState.Escape); }
			if (config[keybind] == "ImeConvert") { return (m_ID->m_keyboardState.ImeConvert && !m_ID->m_prevKeyboardState.ImeConvert); }
			if (config[keybind] == "ImeNoConvert") { return (m_ID->m_keyboardState.ImeNoConvert && !m_ID->m_prevKeyboardState.ImeNoConvert); }
			if (config[keybind] == "Space") { return (m_ID->m_keyboardState.Space && !m_ID->m_prevKeyboardState.Space); }
			if (config[keybind] == "PageUp") { return (m_ID->m_keyboardState.PageUp && !m_ID->m_prevKeyboardState.PageUp); }
			if (config[keybind] == "PageDown") { return (m_ID->m_keyboardState.PageDown && !m_ID->m_prevKeyboardState.PageDown); }
			if (config[keybind] == "End") { return (m_ID->m_keyboardState.End && !m_ID->m_prevKeyboardState.End); }
			if (config[keybind] == "Home") { return (m_ID->m_keyboardState.Home && !m_ID->m_prevKeyboardState.Home); }
			if (config[keybind] == "Left") { return (m_ID->m_keyboardState.Left && !m_ID->m_prevKeyboardState.Left); }
			if (config[keybind] == "Up") { return (m_ID->m_keyboardState.Up && !m_ID->m_prevKeyboardState.Up); }
			if (config[keybind] == "Right") { return (m_ID->m_keyboardState.Right && !m_ID->m_prevKeyboardState.Right); }
			if (config[keybind] == "Down") { return (m_ID->m_keyboardState.Down && !m_ID->m_prevKeyboardState.Down); }
			if (config[keybind] == "Select") { return (m_ID->m_keyboardState.Select && !m_ID->m_prevKeyboardState.Select); }
			if (config[keybind] == "Print") { return (m_ID->m_keyboardState.Print && !m_ID->m_prevKeyboardState.Print); }
			if (config[keybind] == "Execute") { return (m_ID->m_keyboardState.Execute && !m_ID->m_prevKeyboardState.Execute); }
			if (config[keybind] == "PrintScreen") { return (m_ID->m_keyboardState.PrintScreen && !m_ID->m_prevKeyboardState.PrintScreen); }
			if (config[keybind] == "Insert") { return (m_ID->m_keyboardState.Insert && !m_ID->m_prevKeyboardState.Insert); }
			if (config[keybind] == "Delete") { return (m_ID->m_keyboardState.Delete && !m_ID->m_prevKeyboardState.Delete); }
			if (config[keybind] == "Help") { return (m_ID->m_keyboardState.Help && !m_ID->m_prevKeyboardState.Help); }
			if (config[keybind] == "D0") { return (m_ID->m_keyboardState.D0 && !m_ID->m_prevKeyboardState.D0); }
			if (config[keybind] == "D1") { return (m_ID->m_keyboardState.D1 && !m_ID->m_prevKeyboardState.D1); }
			if (config[keybind] == "D2") { return (m_ID->m_keyboardState.D2 && !m_ID->m_prevKeyboardState.D2); }
			if (config[keybind] == "D3") { return (m_ID->m_keyboardState.D3 && !m_ID->m_prevKeyboardState.D3); }
			if (config[keybind] == "D4") { return (m_ID->m_keyboardState.D4 && !m_ID->m_prevKeyboardState.D4); }
			if (config[keybind] == "D5") { return (m_ID->m_keyboardState.D5 && !m_ID->m_prevKeyboardState.D5); }
			if (config[keybind] == "D6") { return (m_ID->m_keyboardState.D6 && !m_ID->m_prevKeyboardState.D6); }
			if (config[keybind] == "D7") { return (m_ID->m_keyboardState.D7 && !m_ID->m_prevKeyboardState.D7); }
			if (config[keybind] == "D8") { return (m_ID->m_keyboardState.D8 && !m_ID->m_prevKeyboardState.D8); }
			if (config[keybind] == "D9") { return (m_ID->m_keyboardState.D9 && !m_ID->m_prevKeyboardState.D9); }
			if (config[keybind] == "A") { return (m_ID->m_keyboardState.A && !m_ID->m_prevKeyboardState.A); }
			if (config[keybind] == "B") { return (m_ID->m_keyboardState.B && !m_ID->m_prevKeyboardState.B); }
			if (config[keybind] == "C") { return (m_ID->m_keyboardState.C && !m_ID->m_prevKeyboardState.C); }
			if (config[keybind] == "D") { return (m_ID->m_keyboardState.D && !m_ID->m_prevKeyboardState.D); }
			if (config[keybind] == "E") { return (m_ID->m_keyboardState.E && !m_ID->m_prevKeyboardState.E); }
			if (config[keybind] == "F") { return (m_ID->m_keyboardState.F && !m_ID->m_prevKeyboardState.F); }
			if (config[keybind] == "G") { return (m_ID->m_keyboardState.G && !m_ID->m_prevKeyboardState.G); }
			if (config[keybind] == "H") { return (m_ID->m_keyboardState.H && !m_ID->m_prevKeyboardState.H); }
			if (config[keybind] == "I") { return (m_ID->m_keyboardState.I && !m_ID->m_prevKeyboardState.I); }
			if (config[keybind] == "J") { return (m_ID->m_keyboardState.J && !m_ID->m_prevKeyboardState.J); }
			if (config[keybind] == "K") { return (m_ID->m_keyboardState.K && !m_ID->m_prevKeyboardState.K); }
			if (config[keybind] == "L") { return (m_ID->m_keyboardState.L && !m_ID->m_prevKeyboardState.L); }
			if (config[keybind] == "M") { return (m_ID->m_keyboardState.M && !m_ID->m_prevKeyboardState.M); }
			if (config[keybind] == "N") { return (m_ID->m_keyboardState.N && !m_ID->m_prevKeyboardState.N); }
			if (config[keybind] == "O") { return (m_ID->m_keyboardState.O && !m_ID->m_prevKeyboardState.O); }
			if (config[keybind] == "P") { return (m_ID->m_keyboardState.P && !m_ID->m_prevKeyboardState.P); }
			if (config[keybind] == "Q") { return (m_ID->m_keyboardState.Q && !m_ID->m_prevKeyboardState.Q); }
			if (config[keybind] == "R") { return (m_ID->m_keyboardState.R && !m_ID->m_prevKeyboardState.R); }
			if (config[keybind] == "S") { return (m_ID->m_keyboardState.S && !m_ID->m_prevKeyboardState.S); }
			if (config[keybind] == "T") { return (m_ID->m_keyboardState.T && !m_ID->m_prevKeyboardState.T); }
			if (config[keybind] == "U") { return (m_ID->m_keyboardState.U && !m_ID->m_prevKeyboardState.U); }
			if (config[keybind] == "V") { return (m_ID->m_keyboardState.V && !m_ID->m_prevKeyboardState.V); }
			if (config[keybind] == "W") { return (m_ID->m_keyboardState.W && !m_ID->m_prevKeyboardState.W); }
			if (config[keybind] == "X") { return (m_ID->m_keyboardState.X && !m_ID->m_prevKeyboardState.X); }
			if (config[keybind] == "Y") { return (m_ID->m_keyboardState.Y && !m_ID->m_prevKeyboardState.Y); }
			if (config[keybind] == "Z") { return (m_ID->m_keyboardState.Z && !m_ID->m_prevKeyboardState.Z); }
			if (config[keybind] == "LeftWindows") { return (m_ID->m_keyboardState.LeftWindows && !m_ID->m_prevKeyboardState.LeftWindows); }
			if (config[keybind] == "RightWindows") { return (m_ID->m_keyboardState.RightWindows && !m_ID->m_prevKeyboardState.RightWindows); }
			if (config[keybind] == "Apps") { return (m_ID->m_keyboardState.Apps && !m_ID->m_prevKeyboardState.Apps); }
			if (config[keybind] == "Sleep") { return (m_ID->m_keyboardState.Sleep && !m_ID->m_prevKeyboardState.Sleep); }
			if (config[keybind] == "NumPad0") { return (m_ID->m_keyboardState.NumPad0 && !m_ID->m_prevKeyboardState.NumPad0); }
			if (config[keybind] == "NumPad1") { return (m_ID->m_keyboardState.NumPad1 && !m_ID->m_prevKeyboardState.NumPad1); }
			if (config[keybind] == "NumPad2") { return (m_ID->m_keyboardState.NumPad2 && !m_ID->m_prevKeyboardState.NumPad2); }
			if (config[keybind] == "NumPad3") { return (m_ID->m_keyboardState.NumPad3 && !m_ID->m_prevKeyboardState.NumPad3); }
			if (config[keybind] == "NumPad4") { return (m_ID->m_keyboardState.NumPad4 && !m_ID->m_prevKeyboardState.NumPad4); }
			if (config[keybind] == "NumPad5") { return (m_ID->m_keyboardState.NumPad5 && !m_ID->m_prevKeyboardState.NumPad5); }
			if (config[keybind] == "NumPad6") { return (m_ID->m_keyboardState.NumPad6 && !m_ID->m_prevKeyboardState.NumPad6); }
			if (config[keybind] == "NumPad7") { return (m_ID->m_keyboardState.NumPad7 && !m_ID->m_prevKeyboardState.NumPad7); }
			if (config[keybind] == "NumPad8") { return (m_ID->m_keyboardState.NumPad8 && !m_ID->m_prevKeyboardState.NumPad8); }
			if (config[keybind] == "NumPad9") { return (m_ID->m_keyboardState.NumPad9 && !m_ID->m_prevKeyboardState.NumPad9); }
			if (config[keybind] == "Multiply") { return (m_ID->m_keyboardState.Multiply && !m_ID->m_prevKeyboardState.Multiply); }
			if (config[keybind] == "Add") { return (m_ID->m_keyboardState.Add && !m_ID->m_prevKeyboardState.Add); }
			if (config[keybind] == "Separator") { return (m_ID->m_keyboardState.Separator && !m_ID->m_prevKeyboardState.Separator); }
			if (config[keybind] == "Subtract") { return (m_ID->m_keyboardState.Subtract && !m_ID->m_prevKeyboardState.Subtract); }
			if (config[keybind] == "Decimal") { return (m_ID->m_keyboardState.Decimal && !m_ID->m_prevKeyboardState.Decimal); }
			if (config[keybind] == "Divide") { return (m_ID->m_keyboardState.Divide && !m_ID->m_prevKeyboardState.Divide); }
			if (config[keybind] == "F1") { return (m_ID->m_keyboardState.F1 && !m_ID->m_prevKeyboardState.F1); }
			if (config[keybind] == "F2") { return (m_ID->m_keyboardState.F2 && !m_ID->m_prevKeyboardState.F2); }
			if (config[keybind] == "F3") { return (m_ID->m_keyboardState.F3 && !m_ID->m_prevKeyboardState.F3); }
			if (config[keybind] == "F4") { return (m_ID->m_keyboardState.F4 && !m_ID->m_prevKeyboardState.F4); }
			if (config[keybind] == "F5") { return (m_ID->m_keyboardState.F5 && !m_ID->m_prevKeyboardState.F5); }
			if (config[keybind] == "F6") { return (m_ID->m_keyboardState.F6 && !m_ID->m_prevKeyboardState.F6); }
			if (config[keybind] == "F7") { return (m_ID->m_keyboardState.F7 && !m_ID->m_prevKeyboardState.F7); }
			if (config[keybind] == "F8") { return (m_ID->m_keyboardState.F8 && !m_ID->m_prevKeyboardState.F8); }
			if (config[keybind] == "F9") { return (m_ID->m_keyboardState.F9 && !m_ID->m_prevKeyboardState.F9); }
			if (config[keybind] == "F10") { return (m_ID->m_keyboardState.F10 && !m_ID->m_prevKeyboardState.F10); }
			if (config[keybind] == "F11") { return (m_ID->m_keyboardState.F11 && !m_ID->m_prevKeyboardState.F11); }
			if (config[keybind] == "F12") { return (m_ID->m_keyboardState.F12 && !m_ID->m_prevKeyboardState.F12); }
			if (config[keybind] == "F13") { return (m_ID->m_keyboardState.F13 && !m_ID->m_prevKeyboardState.F13); }
			if (config[keybind] == "F14") { return (m_ID->m_keyboardState.F14 && !m_ID->m_prevKeyboardState.F14); }
			if (config[keybind] == "F15") { return (m_ID->m_keyboardState.F15 && !m_ID->m_prevKeyboardState.F15); }
			if (config[keybind] == "F16") { return (m_ID->m_keyboardState.F16 && !m_ID->m_prevKeyboardState.F16); }
			if (config[keybind] == "F17") { return (m_ID->m_keyboardState.F17 && !m_ID->m_prevKeyboardState.F17); }
			if (config[keybind] == "F18") { return (m_ID->m_keyboardState.F18 && !m_ID->m_prevKeyboardState.F18); }
			if (config[keybind] == "F19") { return (m_ID->m_keyboardState.F19 && !m_ID->m_prevKeyboardState.F19); }
			if (config[keybind] == "F20") { return (m_ID->m_keyboardState.F20 && !m_ID->m_prevKeyboardState.F20); }
			if (config[keybind] == "F21") { return (m_ID->m_keyboardState.F21 && !m_ID->m_prevKeyboardState.F21); }
			if (config[keybind] == "F22") { return (m_ID->m_keyboardState.F22 && !m_ID->m_prevKeyboardState.F22); }
			if (config[keybind] == "F23") { return (m_ID->m_keyboardState.F23 && !m_ID->m_prevKeyboardState.F23); }
			if (config[keybind] == "F24") { return (m_ID->m_keyboardState.F24 && !m_ID->m_prevKeyboardState.F24); }
			if (config[keybind] == "NumLock") { return (m_ID->m_keyboardState.NumLock && !m_ID->m_prevKeyboardState.NumLock); }
			if (config[keybind] == "Scroll") { return (m_ID->m_keyboardState.Scroll && !m_ID->m_prevKeyboardState.Scroll); }
			if (config[keybind] == "LeftShift") { return (m_ID->m_keyboardState.LeftShift && !m_ID->m_prevKeyboardState.LeftShift); }
			if (config[keybind] == "RightShift") { return (m_ID->m_keyboardState.RightShift && !m_ID->m_prevKeyboardState.RightShift); }
			if (config[keybind] == "LeftControl") { return (m_ID->m_keyboardState.LeftControl && !m_ID->m_prevKeyboardState.LeftControl); }
			if (config[keybind] == "RightControl") { return (m_ID->m_keyboardState.RightControl && !m_ID->m_prevKeyboardState.RightControl); }
			if (config[keybind] == "LeftAlt") { return (m_ID->m_keyboardState.LeftAlt && !m_ID->m_prevKeyboardState.LeftAlt); }
			if (config[keybind] == "RightAlt") { return (m_ID->m_keyboardState.RightAlt && !m_ID->m_prevKeyboardState.RightAlt); }
			if (config[keybind] == "BrowserBack") { return (m_ID->m_keyboardState.BrowserBack && !m_ID->m_prevKeyboardState.BrowserBack); }
			if (config[keybind] == "BrowserForward") { return (m_ID->m_keyboardState.BrowserForward && !m_ID->m_prevKeyboardState.BrowserForward); }
			if (config[keybind] == "BrowserRefresh") { return (m_ID->m_keyboardState.BrowserRefresh && !m_ID->m_prevKeyboardState.BrowserRefresh); }
			if (config[keybind] == "BrowserStop") { return (m_ID->m_keyboardState.BrowserStop && !m_ID->m_prevKeyboardState.BrowserStop); }
			if (config[keybind] == "BrowserSearch") { return (m_ID->m_keyboardState.BrowserSearch && !m_ID->m_prevKeyboardState.BrowserSearch); }
			if (config[keybind] == "BrowserFavorites") { return (m_ID->m_keyboardState.BrowserFavorites && !m_ID->m_prevKeyboardState.BrowserFavorites); }
			if (config[keybind] == "BrowserHome") { return (m_ID->m_keyboardState.BrowserHome && !m_ID->m_prevKeyboardState.BrowserHome); }
			if (config[keybind] == "VolumeMute") { return (m_ID->m_keyboardState.VolumeMute && !m_ID->m_prevKeyboardState.VolumeMute); }
			if (config[keybind] == "VolumeDown") { return (m_ID->m_keyboardState.VolumeDown && !m_ID->m_prevKeyboardState.VolumeDown); }
			if (config[keybind] == "VolumeUp") { return (m_ID->m_keyboardState.VolumeUp && !m_ID->m_prevKeyboardState.VolumeUp); }
			if (config[keybind] == "MediaNextTrack") { return (m_ID->m_keyboardState.MediaNextTrack && !m_ID->m_prevKeyboardState.MediaNextTrack); }
			if (config[keybind] == "MediaPreviousTrack") { return (m_ID->m_keyboardState.MediaPreviousTrack && !m_ID->m_prevKeyboardState.MediaPreviousTrack); }
			if (config[keybind] == "MediaStop") { return (m_ID->m_keyboardState.MediaStop && !m_ID->m_prevKeyboardState.MediaStop); }
			if (config[keybind] == "MediaPlayPause") { return (m_ID->m_keyboardState.MediaPlayPause && !m_ID->m_prevKeyboardState.MediaPlayPause); }
			if (config[keybind] == "LaunchMail") { return (m_ID->m_keyboardState.LaunchMail && !m_ID->m_prevKeyboardState.LaunchMail); }
			if (config[keybind] == "SelectMedia") { return (m_ID->m_keyboardState.SelectMedia && !m_ID->m_prevKeyboardState.SelectMedia); }
			if (config[keybind] == "LaunchApplication1") { return (m_ID->m_keyboardState.LaunchApplication1 && !m_ID->m_prevKeyboardState.LaunchApplication1); }
			if (config[keybind] == "LaunchApplication2") { return (m_ID->m_keyboardState.LaunchApplication2 && !m_ID->m_prevKeyboardState.LaunchApplication2); }
			if (config[keybind] == "OemSemicolon") { return (m_ID->m_keyboardState.OemSemicolon && !m_ID->m_prevKeyboardState.OemSemicolon); }
			if (config[keybind] == "OemPlus") { return (m_ID->m_keyboardState.OemPlus && !m_ID->m_prevKeyboardState.OemPlus); }
			if (config[keybind] == "OemComma") { return (m_ID->m_keyboardState.OemComma && !m_ID->m_prevKeyboardState.OemComma); }
			if (config[keybind] == "OemMinus") { return (m_ID->m_keyboardState.OemMinus && !m_ID->m_prevKeyboardState.OemMinus); }
			if (config[keybind] == "OemPeriod") { return (m_ID->m_keyboardState.OemPeriod && !m_ID->m_prevKeyboardState.OemPeriod); }
			if (config[keybind] == "OemQuestion") { return (m_ID->m_keyboardState.OemQuestion && !m_ID->m_prevKeyboardState.OemQuestion); }
			if (config[keybind] == "OemTilde") { return (m_ID->m_keyboardState.OemTilde && !m_ID->m_prevKeyboardState.OemTilde); }
			if (config[keybind] == "OemOpenBrackets") { return (m_ID->m_keyboardState.OemOpenBrackets && !m_ID->m_prevKeyboardState.OemOpenBrackets); }
			if (config[keybind] == "OemPipe") { return (m_ID->m_keyboardState.OemPipe && !m_ID->m_prevKeyboardState.OemPipe); }
			if (config[keybind] == "OemCloseBrackets") { return (m_ID->m_keyboardState.OemCloseBrackets && !m_ID->m_prevKeyboardState.OemCloseBrackets); }
			if (config[keybind] == "OemQuotes") { return (m_ID->m_keyboardState.OemQuotes && !m_ID->m_prevKeyboardState.OemQuotes); }
			if (config[keybind] == "Oem8") { return (m_ID->m_keyboardState.Oem8 && !m_ID->m_prevKeyboardState.Oem8); }
			if (config[keybind] == "OemBackslash") { return (m_ID->m_keyboardState.OemBackslash && !m_ID->m_prevKeyboardState.OemBackslash); }
			if (config[keybind] == "ProcessKey") { return (m_ID->m_keyboardState.ProcessKey && !m_ID->m_prevKeyboardState.ProcessKey); }
			if (config[keybind] == "OemCopy") { return (m_ID->m_keyboardState.OemCopy && !m_ID->m_prevKeyboardState.OemCopy); }
			if (config[keybind] == "OemAuto") { return (m_ID->m_keyboardState.OemAuto && !m_ID->m_prevKeyboardState.OemAuto); }
			if (config[keybind] == "OemEnlW") { return (m_ID->m_keyboardState.OemEnlW && !m_ID->m_prevKeyboardState.OemEnlW); }
			if (config[keybind] == "Attn") { return (m_ID->m_keyboardState.Attn && !m_ID->m_prevKeyboardState.Attn); }
			if (config[keybind] == "Crsel") { return (m_ID->m_keyboardState.Crsel && !m_ID->m_prevKeyboardState.Crsel); }
			if (config[keybind] == "Exsel") { return (m_ID->m_keyboardState.Exsel && !m_ID->m_prevKeyboardState.Exsel); }
			if (config[keybind] == "EraseEof") { return (m_ID->m_keyboardState.EraseEof && !m_ID->m_prevKeyboardState.EraseEof); }
			if (config[keybind] == "Play") { return (m_ID->m_keyboardState.Play && !m_ID->m_prevKeyboardState.Play); }
			if (config[keybind] == "Zoom") { return (m_ID->m_keyboardState.Zoom && !m_ID->m_prevKeyboardState.Zoom); }
			if (config[keybind] == "Pa1") { return (m_ID->m_keyboardState.Pa1 && !m_ID->m_prevKeyboardState.Pa1); }
			if (config[keybind] == "OemClear") { return (m_ID->m_keyboardState.OemClear && !m_ID->m_prevKeyboardState.OemClear); }
		}
	}

	// Check to see if key is being held
	bool keyHeld(const std::string& keybind)
	{
		if (checkKeybind(keybind))
		{
			if (config[keybind] == "Back") { return (m_ID->m_keyboardState.Back); }
			if (config[keybind] == "Tab") { return (m_ID->m_keyboardState.Tab); }
			if (config[keybind] == "Enter") { return (m_ID->m_keyboardState.Enter); }
			if (config[keybind] == "Pause") { return (m_ID->m_keyboardState.Pause); }
			if (config[keybind] == "CapsLock") { return (m_ID->m_keyboardState.CapsLock); }
			if (config[keybind] == "Kana") { return (m_ID->m_keyboardState.Kana); }
			if (config[keybind] == "Kanji") { return (m_ID->m_keyboardState.Kanji); }
			if (config[keybind] == "Escape") { return (m_ID->m_keyboardState.Escape); }
			if (config[keybind] == "ImeConvert") { return (m_ID->m_keyboardState.ImeConvert); }
			if (config[keybind] == "ImeNoConvert") { return (m_ID->m_keyboardState.ImeNoConvert); }
			if (config[keybind] == "Space") { return (m_ID->m_keyboardState.Space); }
			if (config[keybind] == "PageUp") { return (m_ID->m_keyboardState.PageUp); }
			if (config[keybind] == "PageDown") { return (m_ID->m_keyboardState.PageDown); }
			if (config[keybind] == "End") { return (m_ID->m_keyboardState.End); }
			if (config[keybind] == "Home") { return (m_ID->m_keyboardState.Home); }
			if (config[keybind] == "Left") { return (m_ID->m_keyboardState.Left); }
			if (config[keybind] == "Up") { return (m_ID->m_keyboardState.Up); }
			if (config[keybind] == "Right") { return (m_ID->m_keyboardState.Right); }
			if (config[keybind] == "Down") { return (m_ID->m_keyboardState.Down); }
			if (config[keybind] == "Select") { return (m_ID->m_keyboardState.Select); }
			if (config[keybind] == "Print") { return (m_ID->m_keyboardState.Print); }
			if (config[keybind] == "Execute") { return (m_ID->m_keyboardState.Execute); }
			if (config[keybind] == "PrintScreen") { return (m_ID->m_keyboardState.PrintScreen); }
			if (config[keybind] == "Insert") { return (m_ID->m_keyboardState.Insert); }
			if (config[keybind] == "Delete") { return (m_ID->m_keyboardState.Delete); }
			if (config[keybind] == "Help") { return (m_ID->m_keyboardState.Help); }
			if (config[keybind] == "D0") { return (m_ID->m_keyboardState.D0); }
			if (config[keybind] == "D1") { return (m_ID->m_keyboardState.D1); }
			if (config[keybind] == "D2") { return (m_ID->m_keyboardState.D2); }
			if (config[keybind] == "D3") { return (m_ID->m_keyboardState.D3); }
			if (config[keybind] == "D4") { return (m_ID->m_keyboardState.D4); }
			if (config[keybind] == "D5") { return (m_ID->m_keyboardState.D5); }
			if (config[keybind] == "D6") { return (m_ID->m_keyboardState.D6); }
			if (config[keybind] == "D7") { return (m_ID->m_keyboardState.D7); }
			if (config[keybind] == "D8") { return (m_ID->m_keyboardState.D8); }
			if (config[keybind] == "D9") { return (m_ID->m_keyboardState.D9); }
			if (config[keybind] == "A") { return (m_ID->m_keyboardState.A); }
			if (config[keybind] == "B") { return (m_ID->m_keyboardState.B); }
			if (config[keybind] == "C") { return (m_ID->m_keyboardState.C); }
			if (config[keybind] == "D") { return (m_ID->m_keyboardState.D); }
			if (config[keybind] == "E") { return (m_ID->m_keyboardState.E); }
			if (config[keybind] == "F") { return (m_ID->m_keyboardState.F); }
			if (config[keybind] == "G") { return (m_ID->m_keyboardState.G); }
			if (config[keybind] == "H") { return (m_ID->m_keyboardState.H); }
			if (config[keybind] == "I") { return (m_ID->m_keyboardState.I); }
			if (config[keybind] == "J") { return (m_ID->m_keyboardState.J); }
			if (config[keybind] == "K") { return (m_ID->m_keyboardState.K); }
			if (config[keybind] == "L") { return (m_ID->m_keyboardState.L); }
			if (config[keybind] == "M") { return (m_ID->m_keyboardState.M); }
			if (config[keybind] == "N") { return (m_ID->m_keyboardState.N); }
			if (config[keybind] == "O") { return (m_ID->m_keyboardState.O); }
			if (config[keybind] == "P") { return (m_ID->m_keyboardState.P); }
			if (config[keybind] == "Q") { return (m_ID->m_keyboardState.Q); }
			if (config[keybind] == "R") { return (m_ID->m_keyboardState.R); }
			if (config[keybind] == "S") { return (m_ID->m_keyboardState.S); }
			if (config[keybind] == "T") { return (m_ID->m_keyboardState.T); }
			if (config[keybind] == "U") { return (m_ID->m_keyboardState.U); }
			if (config[keybind] == "V") { return (m_ID->m_keyboardState.V); }
			if (config[keybind] == "W") { return (m_ID->m_keyboardState.W); }
			if (config[keybind] == "X") { return (m_ID->m_keyboardState.X); }
			if (config[keybind] == "Y") { return (m_ID->m_keyboardState.Y); }
			if (config[keybind] == "Z") { return (m_ID->m_keyboardState.Z); }
			if (config[keybind] == "LeftWindows") { return (m_ID->m_keyboardState.LeftWindows); }
			if (config[keybind] == "RightWindows") { return (m_ID->m_keyboardState.RightWindows); }
			if (config[keybind] == "Apps") { return (m_ID->m_keyboardState.Apps); }
			if (config[keybind] == "Sleep") { return (m_ID->m_keyboardState.Sleep); }
			if (config[keybind] == "NumPad0") { return (m_ID->m_keyboardState.NumPad0); }
			if (config[keybind] == "NumPad1") { return (m_ID->m_keyboardState.NumPad1); }
			if (config[keybind] == "NumPad2") { return (m_ID->m_keyboardState.NumPad2); }
			if (config[keybind] == "NumPad3") { return (m_ID->m_keyboardState.NumPad3); }
			if (config[keybind] == "NumPad4") { return (m_ID->m_keyboardState.NumPad4); }
			if (config[keybind] == "NumPad5") { return (m_ID->m_keyboardState.NumPad5); }
			if (config[keybind] == "NumPad6") { return (m_ID->m_keyboardState.NumPad6); }
			if (config[keybind] == "NumPad7") { return (m_ID->m_keyboardState.NumPad7); }
			if (config[keybind] == "NumPad8") { return (m_ID->m_keyboardState.NumPad8); }
			if (config[keybind] == "NumPad9") { return (m_ID->m_keyboardState.NumPad9); }
			if (config[keybind] == "Multiply") { return (m_ID->m_keyboardState.Multiply); }
			if (config[keybind] == "Add") { return (m_ID->m_keyboardState.Add); }
			if (config[keybind] == "Separator") { return (m_ID->m_keyboardState.Separator); }
			if (config[keybind] == "Subtract") { return (m_ID->m_keyboardState.Subtract); }
			if (config[keybind] == "Decimal") { return (m_ID->m_keyboardState.Decimal); }
			if (config[keybind] == "Divide") { return (m_ID->m_keyboardState.Divide); }
			if (config[keybind] == "F1") { return (m_ID->m_keyboardState.F1); }
			if (config[keybind] == "F2") { return (m_ID->m_keyboardState.F2); }
			if (config[keybind] == "F3") { return (m_ID->m_keyboardState.F3); }
			if (config[keybind] == "F4") { return (m_ID->m_keyboardState.F4); }
			if (config[keybind] == "F5") { return (m_ID->m_keyboardState.F5); }
			if (config[keybind] == "F6") { return (m_ID->m_keyboardState.F6); }
			if (config[keybind] == "F7") { return (m_ID->m_keyboardState.F7); }
			if (config[keybind] == "F8") { return (m_ID->m_keyboardState.F8); }
			if (config[keybind] == "F9") { return (m_ID->m_keyboardState.F9); }
			if (config[keybind] == "F10") { return (m_ID->m_keyboardState.F10); }
			if (config[keybind] == "F11") { return (m_ID->m_keyboardState.F11); }
			if (config[keybind] == "F12") { return (m_ID->m_keyboardState.F12); }
			if (config[keybind] == "F13") { return (m_ID->m_keyboardState.F13); }
			if (config[keybind] == "F14") { return (m_ID->m_keyboardState.F14); }
			if (config[keybind] == "F15") { return (m_ID->m_keyboardState.F15); }
			if (config[keybind] == "F16") { return (m_ID->m_keyboardState.F16); }
			if (config[keybind] == "F17") { return (m_ID->m_keyboardState.F17); }
			if (config[keybind] == "F18") { return (m_ID->m_keyboardState.F18); }
			if (config[keybind] == "F19") { return (m_ID->m_keyboardState.F19); }
			if (config[keybind] == "F20") { return (m_ID->m_keyboardState.F20); }
			if (config[keybind] == "F21") { return (m_ID->m_keyboardState.F21); }
			if (config[keybind] == "F22") { return (m_ID->m_keyboardState.F22); }
			if (config[keybind] == "F23") { return (m_ID->m_keyboardState.F23); }
			if (config[keybind] == "F24") { return (m_ID->m_keyboardState.F24); }
			if (config[keybind] == "NumLock") { return (m_ID->m_keyboardState.NumLock); }
			if (config[keybind] == "Scroll") { return (m_ID->m_keyboardState.Scroll); }
			if (config[keybind] == "LeftShift") { return (m_ID->m_keyboardState.LeftShift); }
			if (config[keybind] == "RightShift") { return (m_ID->m_keyboardState.RightShift); }
			if (config[keybind] == "LeftControl") { return (m_ID->m_keyboardState.LeftControl); }
			if (config[keybind] == "RightControl") { return (m_ID->m_keyboardState.RightControl); }
			if (config[keybind] == "LeftAlt") { return (m_ID->m_keyboardState.LeftAlt); }
			if (config[keybind] == "RightAlt") { return (m_ID->m_keyboardState.RightAlt); }
			if (config[keybind] == "BrowserBack") { return (m_ID->m_keyboardState.BrowserBack); }
			if (config[keybind] == "BrowserForward") { return (m_ID->m_keyboardState.BrowserForward); }
			if (config[keybind] == "BrowserRefresh") { return (m_ID->m_keyboardState.BrowserRefresh); }
			if (config[keybind] == "BrowserStop") { return (m_ID->m_keyboardState.BrowserStop); }
			if (config[keybind] == "BrowserSearch") { return (m_ID->m_keyboardState.BrowserSearch); }
			if (config[keybind] == "BrowserFavorites") { return (m_ID->m_keyboardState.BrowserFavorites); }
			if (config[keybind] == "BrowserHome") { return (m_ID->m_keyboardState.BrowserHome); }
			if (config[keybind] == "VolumeMute") { return (m_ID->m_keyboardState.VolumeMute); }
			if (config[keybind] == "VolumeDown") { return (m_ID->m_keyboardState.VolumeDown); }
			if (config[keybind] == "VolumeUp") { return (m_ID->m_keyboardState.VolumeUp); }
			if (config[keybind] == "MediaNextTrack") { return (m_ID->m_keyboardState.MediaNextTrack); }
			if (config[keybind] == "MediaPreviousTrack") { return (m_ID->m_keyboardState.MediaPreviousTrack); }
			if (config[keybind] == "MediaStop") { return (m_ID->m_keyboardState.MediaStop); }
			if (config[keybind] == "MediaPlayPause") { return (m_ID->m_keyboardState.MediaPlayPause); }
			if (config[keybind] == "LaunchMail") { return (m_ID->m_keyboardState.LaunchMail); }
			if (config[keybind] == "SelectMedia") { return (m_ID->m_keyboardState.SelectMedia); }
			if (config[keybind] == "LaunchApplication1") { return (m_ID->m_keyboardState.LaunchApplication1); }
			if (config[keybind] == "LaunchApplication2") { return (m_ID->m_keyboardState.LaunchApplication2); }
			if (config[keybind] == "OemSemicolon") { return (m_ID->m_keyboardState.OemSemicolon); }
			if (config[keybind] == "OemPlus") { return (m_ID->m_keyboardState.OemPlus); }
			if (config[keybind] == "OemComma") { return (m_ID->m_keyboardState.OemComma); }
			if (config[keybind] == "OemMinus") { return (m_ID->m_keyboardState.OemMinus); }
			if (config[keybind] == "OemPeriod") { return (m_ID->m_keyboardState.OemPeriod); }
			if (config[keybind] == "OemQuestion") { return (m_ID->m_keyboardState.OemQuestion); }
			if (config[keybind] == "OemTilde") { return (m_ID->m_keyboardState.OemTilde); }
			if (config[keybind] == "OemOpenBrackets") { return (m_ID->m_keyboardState.OemOpenBrackets); }
			if (config[keybind] == "OemPipe") { return (m_ID->m_keyboardState.OemPipe); }
			if (config[keybind] == "OemCloseBrackets") { return (m_ID->m_keyboardState.OemCloseBrackets); }
			if (config[keybind] == "OemQuotes") { return (m_ID->m_keyboardState.OemQuotes); }
			if (config[keybind] == "Oem8") { return (m_ID->m_keyboardState.Oem8); }
			if (config[keybind] == "OemBackslash") { return (m_ID->m_keyboardState.OemBackslash); }
			if (config[keybind] == "ProcessKey") { return (m_ID->m_keyboardState.ProcessKey); }
			if (config[keybind] == "OemCopy") { return (m_ID->m_keyboardState.OemCopy); }
			if (config[keybind] == "OemAuto") { return (m_ID->m_keyboardState.OemAuto); }
			if (config[keybind] == "OemEnlW") { return (m_ID->m_keyboardState.OemEnlW); }
			if (config[keybind] == "Attn") { return (m_ID->m_keyboardState.Attn); }
			if (config[keybind] == "Crsel") { return (m_ID->m_keyboardState.Crsel); }
			if (config[keybind] == "Exsel") { return (m_ID->m_keyboardState.Exsel); }
			if (config[keybind] == "EraseEof") { return (m_ID->m_keyboardState.EraseEof); }
			if (config[keybind] == "Play") { return (m_ID->m_keyboardState.Play); }
			if (config[keybind] == "Zoom") { return (m_ID->m_keyboardState.Zoom); }
			if (config[keybind] == "Pa1") { return (m_ID->m_keyboardState.Pa1); }
			if (config[keybind] == "OemClear") { return (m_ID->m_keyboardState.OemClear); }
		}
	}

private:
	bool checkKeybind(const std::string& keybind)
	{
		if (config[keybind].is_null())
		{
			throw std::runtime_error("Attempted to access an undeclared keybind: " + keybind);
		}
		return true;
	}

	static ThICC_InputData* m_ID;
	static json config;
	GameFilepaths m_filepath;
};