#pragma once

#include <stdexcept>

struct ThICC_GraphicsResources;
struct ThICC_ModelResources;

struct Locator {
	static ThICC_GraphicsResources* getGraphicsResources() {
		if (ref_GR == nullptr) {
			throw std::runtime_error("Call to graphics resources before initialisation.");
		}
		return ref_GR;
	}
	static ThICC_ModelResources* getModelResources() {
		if (ref_MR == nullptr) {
			throw std::runtime_error("Call to model resources before initialisation.");
		}
		return ref_MR;
	}

	static void setGraphicsResources(ThICC_GraphicsResources* inst_DR) {
		ref_GR = inst_DR;
	}
	static void setModelResources(ThICC_ModelResources* inst_DR) {
		ref_MR = inst_DR;
	}

private:
	static ThICC_GraphicsResources* ref_GR;
	static ThICC_ModelResources* ref_MR;
};