#include <limits>
#include <fstream>
#include "Results.h"

Results Results::loadResults(const std::string& filename, bool *isValid) {
	std::ifstream results_file(filename);
	Results results;
	size_t size;
	bool valid = true;
	if (!results_file.is_open()) {
		valid = false;
		*isValid = valid;
		return results;
	}
	results_file >> size;
	while (!results_file.eof() && size-- != 0) {
		size_t iteration;
		int result;
		results_file >> iteration >> result;
		results.addResult(iteration, static_cast<ResultValue>(result));
	}
	*isValid = true;
	return results;
}

void Results::saveResults(const std::string& filename) {
	std::ofstream results_file(filename);
	results_file << results.size();
	for (const auto& result : results) {
		results_file << '\n' << result.first << ' ' << result.second;
	}
	results_file.close();
	results.clear();
}

size_t Results::getNextCrashIteration() const {
	if (!results.empty() && results.front().second == crash) {
		return results.front().first;
	}
	else return std::numeric_limits<size_t>::max(); // a big number we will never reach
}
