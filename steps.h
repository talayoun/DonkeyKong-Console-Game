#pragma once

#include <list>
#include "board.h"

class Steps {
	long randomSeed = 0;
	std::list<std::pair<size_t, char>> steps; // pair: iteration, step
public:
	static Steps loadSteps(const std::string& filename, Board& board, bool* isValid);
	void saveSteps(const std::string& filename);
	long getRandomSeed() const {
		return randomSeed;
	}
	void setRandomSeed(long seed) {
		randomSeed = seed;
	}
	void addStep(int iteration, char step) {
		steps.push_back({ iteration, step });
	}
	bool isNextStepOnIteration(size_t iteration) const {
		return !steps.empty() && steps.front().first == iteration;
	}
	char popStep() {
		char step = steps.front().second;
		steps.pop_front();
		return step;
	}
};
