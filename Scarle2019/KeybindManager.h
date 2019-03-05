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
	void setup(GameStateData* _GSD) {
		//Read in config
		std::ifstream i(m_filepath.generateConfigFilepath("Keybinds", m_filepath.CONFIG));
		config << i;
		
		//Save static pointer to keybind event data
		m_GSD = _GSD;
	};

	// Check to see if key was pressed
	bool keyPressed(const std::string& keybind)
	{
		return checkKeybind(keybind);
	}

private:
	bool checkKeybind(const std::string& keybind)
	{
		if (config[keybind].is_null())
		{
			std::cout << "WARNING: ATTEMPTED TO ACCESS UNINITIALISED KEYBIND - " << keybind;
			return false;
		}

		if (config[keybind] == "Back") { return (m_GSD->m_keyboardState.Back && !m_GSD->m_prevKeyboardState.Back); }
		if (config[keybind] == "Back") { return (m_GSD->m_keyboardState.Back && !m_GSD->m_prevKeyboardState.Back); }
		if (config[keybind] == "Tab") { return (m_GSD->m_keyboardState.Tab && !m_GSD->m_prevKeyboardState.Tab); }
		if (config[keybind] == "Enter") { return (m_GSD->m_keyboardState.Enter && !m_GSD->m_prevKeyboardState.Enter); }
		if (config[keybind] == "Pause") { return (m_GSD->m_keyboardState.Pause && !m_GSD->m_prevKeyboardState.Pause); }
		if (config[keybind] == "CapsLock") { return (m_GSD->m_keyboardState.CapsLock && !m_GSD->m_prevKeyboardState.CapsLock); }
		if (config[keybind] == "Kana") { return (m_GSD->m_keyboardState.Kana && !m_GSD->m_prevKeyboardState.Kana); }
		if (config[keybind] == "Kanji") { return (m_GSD->m_keyboardState.Kanji && !m_GSD->m_prevKeyboardState.Kanji); }
		if (config[keybind] == "Escape") { return (m_GSD->m_keyboardState.Escape && !m_GSD->m_prevKeyboardState.Escape); }
		if (config[keybind] == "ImeConvert") { return (m_GSD->m_keyboardState.ImeConvert && !m_GSD->m_prevKeyboardState.ImeConvert); }
		if (config[keybind] == "ImeNoConvert") { return (m_GSD->m_keyboardState.ImeNoConvert && !m_GSD->m_prevKeyboardState.ImeNoConvert); }
		if (config[keybind] == "Space") { return (m_GSD->m_keyboardState.Space && !m_GSD->m_prevKeyboardState.Space); }
		if (config[keybind] == "PageUp") { return (m_GSD->m_keyboardState.PageUp && !m_GSD->m_prevKeyboardState.PageUp); }
		if (config[keybind] == "PageDown") { return (m_GSD->m_keyboardState.PageDown && !m_GSD->m_prevKeyboardState.PageDown); }
		if (config[keybind] == "End") { return (m_GSD->m_keyboardState.End && !m_GSD->m_prevKeyboardState.End); }
		if (config[keybind] == "Home") { return (m_GSD->m_keyboardState.Home && !m_GSD->m_prevKeyboardState.Home); }
		if (config[keybind] == "Left") { return (m_GSD->m_keyboardState.Left && !m_GSD->m_prevKeyboardState.Left); }
		if (config[keybind] == "Up") { return (m_GSD->m_keyboardState.Up && !m_GSD->m_prevKeyboardState.Up); }
		if (config[keybind] == "Right") { return (m_GSD->m_keyboardState.Right && !m_GSD->m_prevKeyboardState.Right); }
		if (config[keybind] == "Down") { return (m_GSD->m_keyboardState.Down && !m_GSD->m_prevKeyboardState.Down); }
		if (config[keybind] == "Select") { return (m_GSD->m_keyboardState.Select && !m_GSD->m_prevKeyboardState.Select); }
		if (config[keybind] == "Print") { return (m_GSD->m_keyboardState.Print && !m_GSD->m_prevKeyboardState.Print); }
		if (config[keybind] == "Execute") { return (m_GSD->m_keyboardState.Execute && !m_GSD->m_prevKeyboardState.Execute); }
		if (config[keybind] == "PrintScreen") { return (m_GSD->m_keyboardState.PrintScreen && !m_GSD->m_prevKeyboardState.PrintScreen); }
		if (config[keybind] == "Insert") { return (m_GSD->m_keyboardState.Insert && !m_GSD->m_prevKeyboardState.Insert); }
		if (config[keybind] == "Delete") { return (m_GSD->m_keyboardState.Delete && !m_GSD->m_prevKeyboardState.Delete); }
		if (config[keybind] == "Help") { return (m_GSD->m_keyboardState.Help && !m_GSD->m_prevKeyboardState.Help); }
		if (config[keybind] == "D0") { return (m_GSD->m_keyboardState.D0 && !m_GSD->m_prevKeyboardState.D0); }
		if (config[keybind] == "D1") { return (m_GSD->m_keyboardState.D1 && !m_GSD->m_prevKeyboardState.D1); }
		if (config[keybind] == "D2") { return (m_GSD->m_keyboardState.D2 && !m_GSD->m_prevKeyboardState.D2); }
		if (config[keybind] == "D3") { return (m_GSD->m_keyboardState.D3 && !m_GSD->m_prevKeyboardState.D3); }
		if (config[keybind] == "D4") { return (m_GSD->m_keyboardState.D4 && !m_GSD->m_prevKeyboardState.D4); }
		if (config[keybind] == "D5") { return (m_GSD->m_keyboardState.D5 && !m_GSD->m_prevKeyboardState.D5); }
		if (config[keybind] == "D6") { return (m_GSD->m_keyboardState.D6 && !m_GSD->m_prevKeyboardState.D6); }
		if (config[keybind] == "D7") { return (m_GSD->m_keyboardState.D7 && !m_GSD->m_prevKeyboardState.D7); }
		if (config[keybind] == "D8") { return (m_GSD->m_keyboardState.D8 && !m_GSD->m_prevKeyboardState.D8); }
		if (config[keybind] == "D9") { return (m_GSD->m_keyboardState.D9 && !m_GSD->m_prevKeyboardState.D9); }
		if (config[keybind] == "A") { return (m_GSD->m_keyboardState.A && !m_GSD->m_prevKeyboardState.A); }
		if (config[keybind] == "B") { return (m_GSD->m_keyboardState.B && !m_GSD->m_prevKeyboardState.B); }
		if (config[keybind] == "C") { return (m_GSD->m_keyboardState.C && !m_GSD->m_prevKeyboardState.C); }
		if (config[keybind] == "D") { return (m_GSD->m_keyboardState.D && !m_GSD->m_prevKeyboardState.D); }
		if (config[keybind] == "E") { return (m_GSD->m_keyboardState.E && !m_GSD->m_prevKeyboardState.E); }
		if (config[keybind] == "F") { return (m_GSD->m_keyboardState.F && !m_GSD->m_prevKeyboardState.F); }
		if (config[keybind] == "G") { return (m_GSD->m_keyboardState.G && !m_GSD->m_prevKeyboardState.G); }
		if (config[keybind] == "H") { return (m_GSD->m_keyboardState.H && !m_GSD->m_prevKeyboardState.H); }
		if (config[keybind] == "I") { return (m_GSD->m_keyboardState.I && !m_GSD->m_prevKeyboardState.I); }
		if (config[keybind] == "J") { return (m_GSD->m_keyboardState.J && !m_GSD->m_prevKeyboardState.J); }
		if (config[keybind] == "K") { return (m_GSD->m_keyboardState.K && !m_GSD->m_prevKeyboardState.K); }
		if (config[keybind] == "L") { return (m_GSD->m_keyboardState.L && !m_GSD->m_prevKeyboardState.L); }
		if (config[keybind] == "M") { return (m_GSD->m_keyboardState.M && !m_GSD->m_prevKeyboardState.M); }
		if (config[keybind] == "N") { return (m_GSD->m_keyboardState.N && !m_GSD->m_prevKeyboardState.N); }
		if (config[keybind] == "O") { return (m_GSD->m_keyboardState.O && !m_GSD->m_prevKeyboardState.O); }
		if (config[keybind] == "P") { return (m_GSD->m_keyboardState.P && !m_GSD->m_prevKeyboardState.P); }
		if (config[keybind] == "Q") { return (m_GSD->m_keyboardState.Q && !m_GSD->m_prevKeyboardState.Q); }
		if (config[keybind] == "R") { return (m_GSD->m_keyboardState.R && !m_GSD->m_prevKeyboardState.R); }
		if (config[keybind] == "S") { return (m_GSD->m_keyboardState.S && !m_GSD->m_prevKeyboardState.S); }
		if (config[keybind] == "T") { return (m_GSD->m_keyboardState.T && !m_GSD->m_prevKeyboardState.T); }
		if (config[keybind] == "U") { return (m_GSD->m_keyboardState.U && !m_GSD->m_prevKeyboardState.U); }
		if (config[keybind] == "V") { return (m_GSD->m_keyboardState.V && !m_GSD->m_prevKeyboardState.V); }
		if (config[keybind] == "W") { return (m_GSD->m_keyboardState.W && !m_GSD->m_prevKeyboardState.W); }
		if (config[keybind] == "X") { return (m_GSD->m_keyboardState.X && !m_GSD->m_prevKeyboardState.X); }
		if (config[keybind] == "Y") { return (m_GSD->m_keyboardState.Y && !m_GSD->m_prevKeyboardState.Y); }
		if (config[keybind] == "Z") { return (m_GSD->m_keyboardState.Z && !m_GSD->m_prevKeyboardState.Z); }
		if (config[keybind] == "LeftWindows") { return (m_GSD->m_keyboardState.LeftWindows && !m_GSD->m_prevKeyboardState.LeftWindows); }
		if (config[keybind] == "RightWindows") { return (m_GSD->m_keyboardState.RightWindows && !m_GSD->m_prevKeyboardState.RightWindows); }
		if (config[keybind] == "Apps") { return (m_GSD->m_keyboardState.Apps && !m_GSD->m_prevKeyboardState.Apps); }
		if (config[keybind] == "Sleep") { return (m_GSD->m_keyboardState.Sleep && !m_GSD->m_prevKeyboardState.Sleep); }
		if (config[keybind] == "NumPad0") { return (m_GSD->m_keyboardState.NumPad0 && !m_GSD->m_prevKeyboardState.NumPad0); }
		if (config[keybind] == "NumPad1") { return (m_GSD->m_keyboardState.NumPad1 && !m_GSD->m_prevKeyboardState.NumPad1); }
		if (config[keybind] == "NumPad2") { return (m_GSD->m_keyboardState.NumPad2 && !m_GSD->m_prevKeyboardState.NumPad2); }
		if (config[keybind] == "NumPad3") { return (m_GSD->m_keyboardState.NumPad3 && !m_GSD->m_prevKeyboardState.NumPad3); }
		if (config[keybind] == "NumPad4") { return (m_GSD->m_keyboardState.NumPad4 && !m_GSD->m_prevKeyboardState.NumPad4); }
		if (config[keybind] == "NumPad5") { return (m_GSD->m_keyboardState.NumPad5 && !m_GSD->m_prevKeyboardState.NumPad5); }
		if (config[keybind] == "NumPad6") { return (m_GSD->m_keyboardState.NumPad6 && !m_GSD->m_prevKeyboardState.NumPad6); }
		if (config[keybind] == "NumPad7") { return (m_GSD->m_keyboardState.NumPad7 && !m_GSD->m_prevKeyboardState.NumPad7); }
		if (config[keybind] == "NumPad8") { return (m_GSD->m_keyboardState.NumPad8 && !m_GSD->m_prevKeyboardState.NumPad8); }
		if (config[keybind] == "NumPad9") { return (m_GSD->m_keyboardState.NumPad9 && !m_GSD->m_prevKeyboardState.NumPad9); }
		if (config[keybind] == "Multiply") { return (m_GSD->m_keyboardState.Multiply && !m_GSD->m_prevKeyboardState.Multiply); }
		if (config[keybind] == "Add") { return (m_GSD->m_keyboardState.Add && !m_GSD->m_prevKeyboardState.Add); }
		if (config[keybind] == "Separator") { return (m_GSD->m_keyboardState.Separator && !m_GSD->m_prevKeyboardState.Separator); }
		if (config[keybind] == "Subtract") { return (m_GSD->m_keyboardState.Subtract && !m_GSD->m_prevKeyboardState.Subtract); }
		if (config[keybind] == "Decimal") { return (m_GSD->m_keyboardState.Decimal && !m_GSD->m_prevKeyboardState.Decimal); }
		if (config[keybind] == "Divide") { return (m_GSD->m_keyboardState.Divide && !m_GSD->m_prevKeyboardState.Divide); }
		if (config[keybind] == "F1") { return (m_GSD->m_keyboardState.F1 && !m_GSD->m_prevKeyboardState.F1); }
		if (config[keybind] == "F2") { return (m_GSD->m_keyboardState.F2 && !m_GSD->m_prevKeyboardState.F2); }
		if (config[keybind] == "F3") { return (m_GSD->m_keyboardState.F3 && !m_GSD->m_prevKeyboardState.F3); }
		if (config[keybind] == "F4") { return (m_GSD->m_keyboardState.F4 && !m_GSD->m_prevKeyboardState.F4); }
		if (config[keybind] == "F5") { return (m_GSD->m_keyboardState.F5 && !m_GSD->m_prevKeyboardState.F5); }
		if (config[keybind] == "F6") { return (m_GSD->m_keyboardState.F6 && !m_GSD->m_prevKeyboardState.F6); }
		if (config[keybind] == "F7") { return (m_GSD->m_keyboardState.F7 && !m_GSD->m_prevKeyboardState.F7); }
		if (config[keybind] == "F8") { return (m_GSD->m_keyboardState.F8 && !m_GSD->m_prevKeyboardState.F8); }
		if (config[keybind] == "F9") { return (m_GSD->m_keyboardState.F9 && !m_GSD->m_prevKeyboardState.F9); }
		if (config[keybind] == "F10") { return (m_GSD->m_keyboardState.F10 && !m_GSD->m_prevKeyboardState.F10); }
		if (config[keybind] == "F11") { return (m_GSD->m_keyboardState.F11 && !m_GSD->m_prevKeyboardState.F11); }
		if (config[keybind] == "F12") { return (m_GSD->m_keyboardState.F12 && !m_GSD->m_prevKeyboardState.F12); }
		if (config[keybind] == "F13") { return (m_GSD->m_keyboardState.F13 && !m_GSD->m_prevKeyboardState.F13); }
		if (config[keybind] == "F14") { return (m_GSD->m_keyboardState.F14 && !m_GSD->m_prevKeyboardState.F14); }
		if (config[keybind] == "F15") { return (m_GSD->m_keyboardState.F15 && !m_GSD->m_prevKeyboardState.F15); }
		if (config[keybind] == "F16") { return (m_GSD->m_keyboardState.F16 && !m_GSD->m_prevKeyboardState.F16); }
		if (config[keybind] == "F17") { return (m_GSD->m_keyboardState.F17 && !m_GSD->m_prevKeyboardState.F17); }
		if (config[keybind] == "F18") { return (m_GSD->m_keyboardState.F18 && !m_GSD->m_prevKeyboardState.F18); }
		if (config[keybind] == "F19") { return (m_GSD->m_keyboardState.F19 && !m_GSD->m_prevKeyboardState.F19); }
		if (config[keybind] == "F20") { return (m_GSD->m_keyboardState.F20 && !m_GSD->m_prevKeyboardState.F20); }
		if (config[keybind] == "F21") { return (m_GSD->m_keyboardState.F21 && !m_GSD->m_prevKeyboardState.F21); }
		if (config[keybind] == "F22") { return (m_GSD->m_keyboardState.F22 && !m_GSD->m_prevKeyboardState.F22); }
		if (config[keybind] == "F23") { return (m_GSD->m_keyboardState.F23 && !m_GSD->m_prevKeyboardState.F23); }
		if (config[keybind] == "F24") { return (m_GSD->m_keyboardState.F24 && !m_GSD->m_prevKeyboardState.F24); }
		if (config[keybind] == "NumLock") { return (m_GSD->m_keyboardState.NumLock && !m_GSD->m_prevKeyboardState.NumLock); }
		if (config[keybind] == "Scroll") { return (m_GSD->m_keyboardState.Scroll && !m_GSD->m_prevKeyboardState.Scroll); }
		if (config[keybind] == "LeftShift") { return (m_GSD->m_keyboardState.LeftShift && !m_GSD->m_prevKeyboardState.LeftShift); }
		if (config[keybind] == "RightShift") { return (m_GSD->m_keyboardState.RightShift && !m_GSD->m_prevKeyboardState.RightShift); }
		if (config[keybind] == "LeftControl") { return (m_GSD->m_keyboardState.LeftControl && !m_GSD->m_prevKeyboardState.LeftControl); }
		if (config[keybind] == "RightControl") { return (m_GSD->m_keyboardState.RightControl && !m_GSD->m_prevKeyboardState.RightControl); }
		if (config[keybind] == "LeftAlt") { return (m_GSD->m_keyboardState.LeftAlt && !m_GSD->m_prevKeyboardState.LeftAlt); }
		if (config[keybind] == "RightAlt") { return (m_GSD->m_keyboardState.RightAlt && !m_GSD->m_prevKeyboardState.RightAlt); }
		if (config[keybind] == "BrowserBack") { return (m_GSD->m_keyboardState.BrowserBack && !m_GSD->m_prevKeyboardState.BrowserBack); }
		if (config[keybind] == "BrowserForward") { return (m_GSD->m_keyboardState.BrowserForward && !m_GSD->m_prevKeyboardState.BrowserForward); }
		if (config[keybind] == "BrowserRefresh") { return (m_GSD->m_keyboardState.BrowserRefresh && !m_GSD->m_prevKeyboardState.BrowserRefresh); }
		if (config[keybind] == "BrowserStop") { return (m_GSD->m_keyboardState.BrowserStop && !m_GSD->m_prevKeyboardState.BrowserStop); }
		if (config[keybind] == "BrowserSearch") { return (m_GSD->m_keyboardState.BrowserSearch && !m_GSD->m_prevKeyboardState.BrowserSearch); }
		if (config[keybind] == "BrowserFavorites") { return (m_GSD->m_keyboardState.BrowserFavorites && !m_GSD->m_prevKeyboardState.BrowserFavorites); }
		if (config[keybind] == "BrowserHome") { return (m_GSD->m_keyboardState.BrowserHome && !m_GSD->m_prevKeyboardState.BrowserHome); }
		if (config[keybind] == "VolumeMute") { return (m_GSD->m_keyboardState.VolumeMute && !m_GSD->m_prevKeyboardState.VolumeMute); }
		if (config[keybind] == "VolumeDown") { return (m_GSD->m_keyboardState.VolumeDown && !m_GSD->m_prevKeyboardState.VolumeDown); }
		if (config[keybind] == "VolumeUp") { return (m_GSD->m_keyboardState.VolumeUp && !m_GSD->m_prevKeyboardState.VolumeUp); }
		if (config[keybind] == "MediaNextTrack") { return (m_GSD->m_keyboardState.MediaNextTrack && !m_GSD->m_prevKeyboardState.MediaNextTrack); }
		if (config[keybind] == "MediaPreviousTrack") { return (m_GSD->m_keyboardState.MediaPreviousTrack && !m_GSD->m_prevKeyboardState.MediaPreviousTrack); }
		if (config[keybind] == "MediaStop") { return (m_GSD->m_keyboardState.MediaStop && !m_GSD->m_prevKeyboardState.MediaStop); }
		if (config[keybind] == "MediaPlayPause") { return (m_GSD->m_keyboardState.MediaPlayPause && !m_GSD->m_prevKeyboardState.MediaPlayPause); }
		if (config[keybind] == "LaunchMail") { return (m_GSD->m_keyboardState.LaunchMail && !m_GSD->m_prevKeyboardState.LaunchMail); }
		if (config[keybind] == "SelectMedia") { return (m_GSD->m_keyboardState.SelectMedia && !m_GSD->m_prevKeyboardState.SelectMedia); }
		if (config[keybind] == "LaunchApplication1") { return (m_GSD->m_keyboardState.LaunchApplication1 && !m_GSD->m_prevKeyboardState.LaunchApplication1); }
		if (config[keybind] == "LaunchApplication2") { return (m_GSD->m_keyboardState.LaunchApplication2 && !m_GSD->m_prevKeyboardState.LaunchApplication2); }
		if (config[keybind] == "OemSemicolon") { return (m_GSD->m_keyboardState.OemSemicolon && !m_GSD->m_prevKeyboardState.OemSemicolon); }
		if (config[keybind] == "OemPlus") { return (m_GSD->m_keyboardState.OemPlus && !m_GSD->m_prevKeyboardState.OemPlus); }
		if (config[keybind] == "OemComma") { return (m_GSD->m_keyboardState.OemComma && !m_GSD->m_prevKeyboardState.OemComma); }
		if (config[keybind] == "OemMinus") { return (m_GSD->m_keyboardState.OemMinus && !m_GSD->m_prevKeyboardState.OemMinus); }
		if (config[keybind] == "OemPeriod") { return (m_GSD->m_keyboardState.OemPeriod && !m_GSD->m_prevKeyboardState.OemPeriod); }
		if (config[keybind] == "OemQuestion") { return (m_GSD->m_keyboardState.OemQuestion && !m_GSD->m_prevKeyboardState.OemQuestion); }
		if (config[keybind] == "OemTilde") { return (m_GSD->m_keyboardState.OemTilde && !m_GSD->m_prevKeyboardState.OemTilde); }
		if (config[keybind] == "OemOpenBrackets") { return (m_GSD->m_keyboardState.OemOpenBrackets && !m_GSD->m_prevKeyboardState.OemOpenBrackets); }
		if (config[keybind] == "OemPipe") { return (m_GSD->m_keyboardState.OemPipe && !m_GSD->m_prevKeyboardState.OemPipe); }
		if (config[keybind] == "OemCloseBrackets") { return (m_GSD->m_keyboardState.OemCloseBrackets && !m_GSD->m_prevKeyboardState.OemCloseBrackets); }
		if (config[keybind] == "OemQuotes") { return (m_GSD->m_keyboardState.OemQuotes && !m_GSD->m_prevKeyboardState.OemQuotes); }
		if (config[keybind] == "Oem8") { return (m_GSD->m_keyboardState.Oem8 && !m_GSD->m_prevKeyboardState.Oem8); }
		if (config[keybind] == "OemBackslash") { return (m_GSD->m_keyboardState.OemBackslash && !m_GSD->m_prevKeyboardState.OemBackslash); }
		if (config[keybind] == "ProcessKey") { return (m_GSD->m_keyboardState.ProcessKey && !m_GSD->m_prevKeyboardState.ProcessKey); }
		if (config[keybind] == "OemCopy") { return (m_GSD->m_keyboardState.OemCopy && !m_GSD->m_prevKeyboardState.OemCopy); }
		if (config[keybind] == "OemAuto") { return (m_GSD->m_keyboardState.OemAuto && !m_GSD->m_prevKeyboardState.OemAuto); }
		if (config[keybind] == "OemEnlW") { return (m_GSD->m_keyboardState.OemEnlW && !m_GSD->m_prevKeyboardState.OemEnlW); }
		if (config[keybind] == "Attn") { return (m_GSD->m_keyboardState.Attn && !m_GSD->m_prevKeyboardState.Attn); }
		if (config[keybind] == "Crsel") { return (m_GSD->m_keyboardState.Crsel && !m_GSD->m_prevKeyboardState.Crsel); }
		if (config[keybind] == "Exsel") { return (m_GSD->m_keyboardState.Exsel && !m_GSD->m_prevKeyboardState.Exsel); }
		if (config[keybind] == "EraseEof") { return (m_GSD->m_keyboardState.EraseEof && !m_GSD->m_prevKeyboardState.EraseEof); }
		if (config[keybind] == "Play") { return (m_GSD->m_keyboardState.Play && !m_GSD->m_prevKeyboardState.Play); }
		if (config[keybind] == "Zoom") { return (m_GSD->m_keyboardState.Zoom && !m_GSD->m_prevKeyboardState.Zoom); }
		if (config[keybind] == "Pa1") { return (m_GSD->m_keyboardState.Pa1 && !m_GSD->m_prevKeyboardState.Pa1); }
		if (config[keybind] == "OemClear") { return (m_GSD->m_keyboardState.OemClear && !m_GSD->m_prevKeyboardState.OemClear); }

		std::cout << "WARNING: KEYBIND DID NOT MATCH ANY KNOWN KEYS - " << keybind;
		return false;
	}

	static GameStateData* m_GSD;
	static json config;
	GameFilepaths m_filepath;
};