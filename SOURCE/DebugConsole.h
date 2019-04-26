#pragma once
#include <string>

class KeybindManager;
class ImageGO2D;
class Text2D;

static class DebugConsole {
public:
	DebugConsole();
	~DebugConsole();

	static void Tick();
	static void Render();

	static bool IsOpen() { return is_visible; }

	static void LogOutput(std::string _log);

private:
	static void UserInputHandles();
	static void AddInputText(std::string _input);
	static void RemoveInputText(int _amount);
	static void SubmitCommand();

	//Engine features
	static KeybindManager* m_keybinds;

	//Background
	static ImageGO2D* console_bg;

	//Text
	static Text2D* input_text;
	static std::vector<std::string> output_text_array;
	static Text2D* output_text;

	//States
	static bool is_visible;
};