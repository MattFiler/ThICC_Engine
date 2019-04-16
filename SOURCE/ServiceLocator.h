#pragma once

#include <stdexcept>

struct ThICC_InputData;
struct ThICC_DeviceData;
struct ThICC_RenderData;
struct ThICC_GameStateData;
class ItemData;
class AIScheduler;
class AudioManager;

struct Locator {
	static ThICC_InputData* getID() {
		if (ref_ID == nullptr) {
			throw std::runtime_error("Call to input data before initialisation.");
		}
		return ref_ID;
	}
	static ThICC_DeviceData* getDD() {
		if (ref_DD == nullptr) {
			throw std::runtime_error("Call to device data before initialisation.");
		}
		return ref_DD;
	}
	static ThICC_RenderData* getRD() {
		if (ref_RD == nullptr) {
			throw std::runtime_error("Call to render data before initialisation.");
		}
		return ref_RD;
	}
	static ThICC_GameStateData* getGSD() {
		if (ref_GSD == nullptr) {
			throw std::runtime_error("Call to gamestate data before initialisation.");
		}
		return ref_GSD;
	}
	static ItemData* getItemData() {
		if (ref_ItemProbability == nullptr) {
			throw std::runtime_error("Call to item data before initialisation.");
		}
		return ref_ItemProbability;
	}
	static AIScheduler* getAIScheduler() {
		if (ref_AIS == nullptr) {
			throw std::runtime_error("Call to AI Scheduler before initialisation.");
		}
		return ref_AIS;
	}
	static AudioManager* getAudio() {
		if (ref_audio == nullptr) {
			throw std::runtime_error("Call to audio before initialisation.");
		}
		return ref_audio;
	}

	static void setupID(ThICC_InputData* inst_ID) {
		ref_ID = inst_ID;
	}
	static void setupDD(ThICC_DeviceData* inst_DD) {
		ref_DD = inst_DD;
	}
	static void setupRD(ThICC_RenderData* inst_RD) {
		ref_RD = inst_RD;
	}
	static void setupGSD(ThICC_GameStateData* inst_GSD) {
		ref_GSD = inst_GSD;
	}
	static void setupItemData(ItemData* inst_ItemProbability) {
		ref_ItemProbability = inst_ItemProbability;
	}
	static void setupAIScheduler(AIScheduler* inst_AIS) {
		ref_AIS = inst_AIS;
	}
	static void setupAudio(AudioManager* inst_audio) {
		ref_audio = inst_audio;
	}

private:
	static ThICC_InputData* ref_ID;
	static ThICC_DeviceData* ref_DD;
	static ThICC_RenderData* ref_RD;
	static ThICC_GameStateData* ref_GSD;
	static ItemData* ref_ItemProbability;
	static AIScheduler* ref_AIS;
	static AudioManager* ref_audio;
};