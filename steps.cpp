#include <fstream>
#include <iostream>
#include "Steps.h"

Steps Steps::loadSteps(const std::string& filename, Board& board, bool* isValid) {
	Steps steps;
	bool valid = true;
	std::ifstream steps_file(filename);
	if (!steps_file.is_open()) {
		valid = false;
		*isValid = valid;
		return steps;  
	}
	steps_file >> steps.randomSeed;
	size_t size;
	steps_file >> size;
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		steps.addStep(iteration, step);
	}
	steps_file.close();
	*isValid = valid;
	return steps;
}

void Steps::saveSteps(const std::string& filename) {
	std::ofstream steps_file(filename);
	steps_file << randomSeed << '\n' << steps.size();
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();
	steps.clear();
}
