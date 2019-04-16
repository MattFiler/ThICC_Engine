#pragma once

#include <stdexcept>

struct ThICC_InputData;

struct Locator {
	static ThICC_InputData* getID() {
		if (ref_ID == nullptr) {
			throw std::runtime_error("Call to input data before initialisation.");
		}
		return ref_ID;
	}

	static void setupID(ThICC_InputData* inst_ID) {
		ref_ID = inst_ID;
	}

private:
	static ThICC_InputData* ref_ID;
};