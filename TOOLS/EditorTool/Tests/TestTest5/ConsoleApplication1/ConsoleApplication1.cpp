// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>

int main()
{
	std::string filepath = "bytes.bin";
	std::ifstream fin(filepath, std::ios::binary);

	if (fin.good()) {
		//Get number of materials in config
		fin.seekg(0);
		int string_length = 0;
		fin.read(reinterpret_cast<char*>(&string_length), sizeof(int));

		char* string = new char[string_length];
		fin.read(string, string_length);

		std::string actual_string;
		actual_string.append(string, string_length);

		std::cout << actual_string;
	}
	else
	{
		std::cout << "COULDN'T OPEN";
	}

	std::string hold;
	std::cin >> hold;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
