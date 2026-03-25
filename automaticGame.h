#pragma once
#include "theGame.h"

class AutomaticGame :public TheGame
{
	bool unmatching_result_found = false;
public:
	AutomaticGame() {}
	void run(Board& board, int choice = NO_COLOR) override;
	void backToMainMenu(Board& board, bool printInSilent = false) override;
};

