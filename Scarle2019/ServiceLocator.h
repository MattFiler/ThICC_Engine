#pragma once

#include <stdexcept>

class RenderData;
class WindowData;
class GameStateData;
class InputData;

struct Locator {
	static RenderData* getRD() {
		if (ref_RD == nullptr) {
			throw std::runtime_error("Call to render data before initialisation.");
		}
		return ref_RD;
	}
	static WindowData* getWD() {
		if (ref_WD == nullptr) {
			throw std::runtime_error("Call to window data before initialisation.");
		}
		return ref_WD;
	}
	static GameStateData* getGSD() {
		if (ref_GSD == nullptr) {
			throw std::runtime_error("Call to game state data before initialisation.");
		}
		return ref_GSD;
	}
	static InputData* getID() {
		if (ref_ID == nullptr) {
			throw std::runtime_error("Call to input data before initialisation.");
		}
		return ref_ID;
	}

	static void setupRD(RenderData* inst_RD) {
		ref_RD = inst_RD;
	}
	static void setupWD(WindowData* inst_WD) {
		ref_WD = inst_WD;
	}
	static void setupGSD(GameStateData* inst_GSD) {
		ref_GSD = inst_GSD;
	}
	static void setupID(InputData* inst_ID) {
		ref_ID = inst_ID;
	}

private:
	static RenderData* ref_RD;
	static WindowData* ref_WD;
	static GameStateData* ref_GSD;
	static InputData* ref_ID;
};