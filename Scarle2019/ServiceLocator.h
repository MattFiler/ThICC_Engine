#pragma once

#include <stdexcept>

class RenderData;
class WindowData;
class GameStateData;
class InputData;
class ItemProbability;

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
	static ItemProbability* getProbabilities() {
		if (ref_ItemProbability == nullptr) {
			throw std::runtime_error("Call to probabilities before initialisation.");
		}
		return ref_ItemProbability;
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
	static void setupProbabilities(ItemProbability* inst_ItemProbability) {
		ref_ItemProbability = inst_ItemProbability;
	}

private:
	static RenderData* ref_RD;
	static WindowData* ref_WD;
	static GameStateData* ref_GSD;
	static InputData* ref_ID;
	static ItemProbability* ref_ItemProbability;
};