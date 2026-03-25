#pragma once
#include "theGame.h"

class ManualGame :public TheGame
{
public:
	ManualGame() {}
	void run(Board& board, int choice = NO_COLOR) override;

};

