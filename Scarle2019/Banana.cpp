#include "pch.h"
#include "Banana.h"
#include <iostream>

Banana::Banana() : Item("BANANA") {
	std::cout << "PROBABILITY FOR FIRST PLACE BANANA: " << m_item_config["probabilities"]["place_1"] << std::endl;
}