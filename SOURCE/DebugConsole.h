#pragma once
#include "ImageGO2D.h"
#include "Text2D.h"
#include "KeybindManager.h"

class DebugConsole {
public:
	DebugConsole();
	~DebugConsole();

	void Tick();
	void Render();

	bool IsOpen() { return is_visible; }

	void LogOutput(std::string _log);

private:
	void UserInputHandles();
	void AddInputText(std::string _input);
	void RemoveInputText(int _amount);
	void SubmitCommand();

	//Engine features
	KeybindManager m_keybinds;

	//Background
	ImageGO2D* console_bg = nullptr;

	//Text
	Text2D* input_text = nullptr;
	std::vector<std::string> output_text_array;
	Text2D* output_text = nullptr;

	//States
	bool is_visible = false;
	const int max_console_lines = 20;
};