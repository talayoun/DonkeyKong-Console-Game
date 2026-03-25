#pragma once

#include "mario.h"
#include "Board.h"
#include "barrel.h"
#include "advancedGhost.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include "gameConfig.h"
#include <vector>
#include <chrono>
#include "results.h"
#include "steps.h"

constexpr int NO_COLOR = 0;
constexpr int COLOR = 1;
constexpr int MESSAGE_X = 29;
constexpr int MESSAGE_Y = 22;
constexpr int GHOST_SCORE = 50;
constexpr int NEXT_LEVEL_SCORE = 100;
constexpr int FIRST = 0;

constexpr int LOAD_SAVE_INDEX = 1;
constexpr int SILENT_INDEX = 2;

static constexpr char RUN_WITHOUT_COLOR = '1';
static constexpr char RUN_WITH_COLOR = '2';
static constexpr char SHOW_GAME_FILES = '3';
static constexpr char SHOW_INSTRUCTIONS = '8';
static constexpr char EXIT = '9';

class TheGame
{
protected:
	Board gameBoard;
	Mario m;
	Barrel barrels[8];
	std::vector<std::unique_ptr<Ghost>> ghosts;

public:
	TheGame();
	bool getAllBoardFileNames(std::vector<std::string>& vec_to_fill, Board& board);
	bool nextLevel(Board& b, Mario& m, Barrel(&_barrels)[BARRELS_ARR_SIZE], const auto& filename, bool& isPaused, bool& lastBarrelExploded, int score, int choice = 0 );
	virtual void backToMainMenu(Board& board, bool printInSilent = false);
	void collision(std::vector<std::unique_ptr<Ghost>>& ghosts);
	int findEnemy(int x, int y, std::vector<std::unique_ptr<Ghost>>& ghosts, Barrel* barrels, int* choice);
	int printFileName(std::vector<std::string> fileNames, size_t* fileIndex);
	void drawScore(bool isSilent, int x, int y, int score, int choice = 0) const;
	void reportResultError(const std::string& message, const std::string& filename, size_t iteration, Board& board) const;
	virtual void run(Board& board, int choice = NO_COLOR);
};


