#pragma once

#include <list>

class Results {
public:
	enum ResultValue { crash, finished, noResult , score};
private:
	std::list<std::pair<int, ResultValue>> results; // pair: iteration or score, result
public:
	static Results loadResults(const std::string& filename,bool* isValid);
	void saveResults(const std::string& filename);
	void addResult(int iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}
	std::pair<size_t, ResultValue> popResult() {
		if (results.empty()) return { 0, Results::noResult };
		auto result = results.front();
		results.pop_front();
		return result;
	}
	bool isFinishedBy(size_t iteration) const {
		return results.empty() || ((results.back().first <= iteration)&&(results.back().second != score));
	}
	size_t getNextCrashIteration() const;
};
