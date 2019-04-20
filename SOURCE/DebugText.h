#pragma once

#include "pch.h"
#include <iostream>
#include <string>

/*

Please route all debug logs to this class so that we don't ship with any!

*/

static struct DebugText {
	static void print(const std::string& message) {
		#ifdef _DEBUG
		std::cout << "ThICC_DEBUG: " << message << std::endl;
		#endif
	}
};