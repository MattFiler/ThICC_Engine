#pragma once
#include "pch.h"
#include <vector>

struct RaceManager
{
public:
	RaceManager() = default;
	void SetRaceAmount(int _number_of_races) { number_of_races = _number_of_races; }
	int GetRaceAmount() { return number_of_races; }
	void SetScore(const std::vector<int>& _scores) { scores = _scores; }
	const std::vector<int>& GetScore() { return scores; }
	std::vector<int>& GetMaps() { return maps; }

	void Reset() {
		is_cup = false;
		current_race_number = 0;
		number_of_races = 0;
		for (std::vector<int>::iterator it = scores.begin(); it != scores.end(); it++)
		{
			(*it) = 0;
		}
		maps.clear();
	}

	int current_race_number = 0;
	bool is_cup = false; // whether it is a cup or a race

private:
	int number_of_races = 0;
	std::vector<int> scores{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::vector<int> maps;
};