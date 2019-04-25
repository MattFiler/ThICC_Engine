#include "pch.h"
#include "DebugConsole.h"
#include "ServiceLocator.h"
#include "InputData.h"

/* Create */
DebugConsole::DebugConsole() {
	//Background
	console_bg = new ImageGO2D("console");

	//User input
	input_text = new Text2D("");
	input_text->SetPos(Vector2(41, 659));
	input_text->SetScale(0.5);
	input_text->SetColour(Colors::Black);

	output_text = new Text2D("");
}

/* Destroy */
DebugConsole::~DebugConsole() {
	console_bg->Reset();
	input_text->Reset();
	output_text->Reset();
}

/* Update */
void DebugConsole::Tick() {
	//Open/close console
	if (m_keybinds.keyReleased("open console")) {
		is_visible = !is_visible;
		input_text->SetText("");
	}

	//User typing
	UserInputHandles();
}

/* Render */
void DebugConsole::Render() {
	if (!is_visible) {
		return;
	}

	console_bg->Render();
	input_text->Render();
	output_text->Render();
}

/* Add a log to the output (typically debug returned from a process) */
void DebugConsole::LogOutput(std::string _log) {
	output_text_array.push_back(_log);
	if (output_text_array.size() > max_console_lines) {
		output_text_array.erase(output_text_array.begin());
	}
	output_text->SetText("");
	for (const std::string& output : output_text_array) {
		output_text->SetText(output_text->GetText() + "\n" + output);
	}
}

/* Handle user inputs */
void DebugConsole::UserInputHandles() {
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::A)) { AddInputText("A"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::B)) { AddInputText("B"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::C)) { AddInputText("C"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::D)) { AddInputText("D"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::E)) { AddInputText("E"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::F)) { AddInputText("F"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::G)) { AddInputText("G"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::H)) { AddInputText("H"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::I)) { AddInputText("I"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::J)) { AddInputText("J"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::K)) { AddInputText("K"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::L)) { AddInputText("L"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::M)) { AddInputText("M"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::N)) { AddInputText("N"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::O)) { AddInputText("O"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::P)) { AddInputText("P"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::Q)) { AddInputText("Q"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::R)) { AddInputText("R"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::S)) { AddInputText("S"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::T)) { AddInputText("T"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::U)) { AddInputText("U"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::V)) { AddInputText("V"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::W)) { AddInputText("W"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::X)) { AddInputText("X"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::Y)) { AddInputText("Y"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::Z)) { AddInputText("Z"); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::Back)) { RemoveInputText(1); }
	if (Locator::getID()->m_keyboardTracker.IsKeyPressed(DirectX::Keyboard::Keys::Enter)) { SubmitCommand(); }
}

/* Add text to input */
void DebugConsole::AddInputText(std::string _input) {
	input_text->SetText(input_text->GetText() + _input);
}

/* Remove a number of characters from the end of the input string */
void DebugConsole::RemoveInputText(int _amount) {
	if (input_text->GetText().length() > _amount) {
		input_text->SetText(input_text->GetText().substr(0, input_text->GetText().length() - _amount));
	}
}

/* Submit input command */
void DebugConsole::SubmitCommand() {
	std::string command = input_text->GetText();

	input_text->SetText("");
}