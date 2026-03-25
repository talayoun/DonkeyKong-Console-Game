#include "theGame.h"

#include <filesystem>

TheGame::TheGame() : m(0, 0, '@') 
{
	for (int i = 0; i < BARRELS_ARR_SIZE; ++i)
	{
		barrels[i] = Barrel(0, 0, 'O');
	}
}

/*Gets the board file names from the path, returns true if matching files were found*/
bool TheGame::getAllBoardFileNames(std::vector<std::string>& vec_to_fill, Board& board) {
	namespace fs = std::filesystem;
	fs::path currentPath = fs::current_path();

	if (!fs::exists(currentPath) || !fs::is_directory(currentPath)) {
		board.resetToInvalidFile();
		board.print();
		gotoxy(20, 9);
		std::cout << "Error - do not exists" << currentPath << std::endl;
		return false;
	}

	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 6) == "dkong_" && filename.extension() == ".screen") {
			vec_to_fill.push_back(filenameStr);
		}
	}
	std::sort(vec_to_fill.begin(), vec_to_fill.end());
	if (vec_to_fill.empty())
	{
		board.resetToInvalidFile();
		board.print();
		gotoxy(20, 9);
		std::cout << "No matching files was found in the directory" << std::endl;
		return false;
	}
	return true;
}

/*Loads the next level board*/
bool TheGame::nextLevel(Board& b, Mario& m, Barrel(& _barrels)[BARRELS_ARR_SIZE], const auto& filename, bool& isPaused, bool& lastBarrelExploded,int score, int choice)
{
	bool isInvalidFile = true;
	isInvalidFile = b.load(filename);
	if (!isInvalidFile)
	{
		m.resetWithHammer();
		b.reset();
		m.resetMarioChar();
		b.printFromFile();
		b.drawPauline(choice);
		b.drawDonkey(choice);
		if (!m.getWithHammer())
			b.drawHammer(b.getHammerChar(), choice);
		m.setBoard(b);
		m.drawHearts(b.getLX(), b.getLY(), choice);
		drawScore(b.getIsSilent(), b.getLX(), b.getLY(), score, choice);
		if (b.isDonkeyExist()) {
			{
				for (int i = 0; i < BARRELS_ARR_SIZE; i++)
				{
					_barrels[i].setBoard(b);
					_barrels[i].setMario(m);
				}
				_barrels[FIRST].setIsMoving(true);
			}
		}
		for (const auto& ghost : b.getGhosts())
		{
			ghost->setBoard(b);
			ghost->setMario(m);
		}
		isPaused = false;
		lastBarrelExploded = false;
		m.resetDir();
		return false;
	}
	return true;

}

void TheGame::backToMainMenu(Board& board, bool printInSilent) {
	if(!board.getIsSilent() || printInSilent)
	{
		for (int i = 5; i >= 1; i--) //Shows back to main menu message with count
		{
			gotoxy(MESSAGE_X, MESSAGE_Y);
			std::cout << "Back to main menu in: " << i;
			Sleep(1000);
		}
		board.resetToStartGame(); //Prints the main menu
		board.print(board.getIsSilent());
	}
}

void TheGame::collision(std::vector<std::unique_ptr<Ghost>> & ghosts) //Check if the ghosts collide
{
	for (size_t i = 0; i < ghosts.size(); ++i) {
		for (size_t j = i + 1; j < ghosts.size(); ++j) {
			auto nextMoveIX = ghosts[i]->getX() + ghosts[i]->getDirX();
			auto nextMoveIY = ghosts[i]->getY();
			auto nextMoveJX = ghosts[j]->getX();
			auto nextMoveJY = ghosts[j]->getY();

			if ((((nextMoveIX == nextMoveJX)&&(ghosts[i]->getDirX()!=ghosts[j]->getDirX())) || (nextMoveIX == (nextMoveJX + ghosts[j]->getDirX()))) && (nextMoveIY == nextMoveJY))
			{
				if (ghosts[i]->getIsMovingRight())
				{
					ghosts[i]->setIsMovingRight(false);
					ghosts[i]->setIsMovingLeft(true);
				}
				else
				{
					ghosts[i]->setIsMovingRight(true);
					ghosts[i]->setIsMovingLeft(false);
				}

				if (ghosts[j]->getIsMovingRight())
				{
					ghosts[j]->setIsMovingRight(false);
					ghosts[j]->setIsMovingLeft(true);
				}
				else
				{
					ghosts[j]->setIsMovingRight(true);
					ghosts[j]->setIsMovingLeft(false);
				}
			}
		}
	}
}

//Returns 0 if mario didn't kill no one, 1 if he killed a barrel or 2 if he killed a ghost
int TheGame::findEnemy(int x, int y, std::vector<std::unique_ptr<Ghost>>& ghosts, Barrel* barrels, int* choice)
{
	bool found = false;
	int index = -1;
	while (!found)
	{
		for (int i = 0; i < BARRELS_ARR_SIZE; i++)
		{
			if ((barrels[i].getX() == x) && (barrels[i].getY() == y))
			{
				found = true;
				index = i;
				*choice = 1;
				return index;
			}
		}
		for (size_t j = 0; j < ghosts.size(); ++j)
		{
			if ((ghosts[j]->getX() == x) && (ghosts[j]->getY() == y))
			{
				found = true;
				index = j;
				*choice = 2;
				return index;
			}
		}
		return index; //Not found
	}
}

//returns 0 if the player wants to run without color, returns 1 otherwise
//updates the chosen file
int TheGame::printFileName(std::vector<std::string> fileNames, size_t* fileIndex)
{
	Board b;
	char index, color;
	const int FILES_PER_PAGE = 5;
	int selectedFile;
	size_t startIndex = FIRST;
	index = '\n';
	while(true)
	{
		b.resetTochooseAFileBoard();
		b.print();
		gotoxy(30, 10);
		for (size_t i = startIndex; i < min((startIndex + FILES_PER_PAGE), fileNames.size()); i++)
		{
			std::cout << (i + 1) << ") " << fileNames[i] << '\n';
			gotoxy(30, 11 + (i - startIndex));
		}
		gotoxy(MESSAGE_X, MESSAGE_Y - 2);
		if (startIndex + FILES_PER_PAGE < fileNames.size())
			std::cout << "Press N to show the next files"<<'\n';
		if (startIndex > FIRST)
			std::cout << "Press B to return to the previous files"<<'\n';
		gotoxy(MESSAGE_X, MESSAGE_Y);
		std::cout << "Press ESC to return to main menu" << '\n';
		gotoxy(11, 10);
		index = _getch();
		if (index == ESC)
			return '\n';
		else if (index == 'N' || index == 'n')
		{
			if (startIndex + FILES_PER_PAGE < fileNames.size())
				startIndex += FILES_PER_PAGE;  
		}
		else if (index == 'B' || index == 'b')
		{
			if (startIndex >= FILES_PER_PAGE)
				startIndex -= FILES_PER_PAGE;  
		}
		else if (index >= '1' && index <= '99')  
		{
			system("cls");
			b.resetTochooseAFileBoard();
			b.print();
			gotoxy(20, 10);
			std::cout << "Run the game with color?" << '\n';
			gotoxy(20, 11);
			std::cout << "(1) Yes                   (2) No" << '\n';
			gotoxy(MESSAGE_X, MESSAGE_Y);
			std::cout << "Press ESC to return to main menu" << '\n';
			color = (int)_getch();
			if (color == ESC)
				return '\n';
			else if (color == '1')
			{
				*fileIndex = (index - '1');
				return COLOR;
			}
			else if (color == '2')
			{
				*fileIndex = (index - '1');
				return NO_COLOR;
			}
			return '\n'; //Flag
		}
	}
}

void TheGame::drawScore(bool isSilent, int x, int y, int score, int choice) const  //Draws score on board
{
	if (!isSilent)
	{
		gotoxy(x + 1, y + 1);
		if (choice == NO_COLOR) //Draws without color
			std::cout << "Score: " << score;
		else if (choice == COLOR)  //Draws with color
		{
			GameConfig g;
			g.setColor(g.RED);
			std::cout << "Score: " << score;
			g.setColor(g.BASIC);
		}
	}
}

void TheGame::reportResultError(const std::string& message, const std::string& filename, size_t iteration, Board& board) const {
	board.resetToTestFailedBoard();
	board.print(board.getIsSilent());
	gotoxy(MESSAGE_X-20, MESSAGE_Y - 4);
	std::cout << "Screen " << filename << " - " << message << '\n';
	gotoxy(MESSAGE_X-20, MESSAGE_Y - 3);
	std::cout << "Iteration: " << iteration << '\n';
	gotoxy(MESSAGE_X-20, MESSAGE_Y - 2);
	std::cout << "Press any key to continue" << std::endl;
	_getch();
}

void TheGame::run(Board& board, int choice)
{
	int index, enemyIndex, runChoice;
	static int score = FIRST;
	std::srand(std::time(0)); // Initializes the random number generator with the current time to ensure unique random numbers
	bool isInvalidFile = false, isPaused = false, lastBarrelExploded = false, firstRun=true;
	static int countingBarrelsOnBoard = FIRST;  //Initializes the barrel count
	std::vector<std::string> fileNames;
	isInvalidFile = !getAllBoardFileNames(fileNames, board);
	if (isInvalidFile) //No valid files
	{
		backToMainMenu(board);
		return;
	}
	size_t currentFileIndex = FIRST;
	if(choice==SHOW_FILES)
	{
		runChoice = printFileName(fileNames, &currentFileIndex);
		if (runChoice == '\n')
		{
			board.resetToStartGame(); //Prints the main menu
			board.print();
			return;
		}
	}
	else
		runChoice = choice;
			for (; currentFileIndex < fileNames.size(); currentFileIndex++)
			{
				const auto& filename = fileNames[currentFileIndex];
				board.setMario(m);
				isInvalidFile = nextLevel(board, m, barrels, filename, isPaused, lastBarrelExploded, score, runChoice);
				if (isInvalidFile && firstRun && ((currentFileIndex + 1) == fileNames.size()))
				{
					gotoxy(MESSAGE_X, MESSAGE_Y - 1);
					std::cout << "No more valid files" << '\n';
					backToMainMenu(board);
					board.resetToStartGame(); //Prints the main menu
					board.print();
					return;
				}
				firstRun = false;
				if (!isInvalidFile) //The file is valid
				{
					if (board.isDonkeyExist()) {
						for (int i = 0; i < BARRELS_ARR_SIZE; i++)
							barrels[i].setEnemy();
						barrels[FIRST].setIsMoving(true);
					}
					for (auto& ghost : board.getGhosts()) 
					{
						ghost->setEnemy();
					}
					countingBarrelsOnBoard = FIRST;  //Initializes the barrel count
					while (true)
					{
						if (m.getIsDead()) //Mario died
						{
							score = score - GHOST_SCORE;
							if (m.getLives() == FIRST) //Mario has no lives left
							{
								board.resetToGameOver(); //Shows game over message
								board.print();
								backToMainMenu(board);
								m.setLives(); //Resets mario's lives
								countingBarrelsOnBoard = FIRST;
								score = FIRST;
								return;
							}
							else  //Mario has lives left
							{
								//Resets the board to starting point settings
								if (board.isDonkeyExist()) {
									for (int i = 0; i < BARRELS_ARR_SIZE; i++)
										barrels[i].setEnemy();
								}
								m.resetWithHammer();
								isInvalidFile = nextLevel(board, m, barrels, fileNames[currentFileIndex], isPaused, lastBarrelExploded, score, runChoice); //We already know that this file is valid
								for (auto& ghost : board.getGhosts()) {
									ghost->setEnemy();
								}
								m.setStartPos(m.getStartX(), m.getStartY());
								countingBarrelsOnBoard = FIRST;
								m.setIsDead();
							}
						}
						if (m.getIsWinning()) //Mario won
						{
							currentFileIndex++;
							score += NEXT_LEVEL_SCORE;
							if (currentFileIndex >= fileNames.size()) //No more files left
							{
								m.setIsWinning(); //Resets mario's to not winning anymore
								board.resetToWinning(); //Prints you won message
								board.printInMiddle();
								backToMainMenu(board);
								m.setLives(); //Resets mario's lives
								countingBarrelsOnBoard = FIRST;
								score = FIRST;
								return;
							}
							else //There are more files
							{
								isInvalidFile = nextLevel(board, m,barrels, fileNames[currentFileIndex], isPaused, lastBarrelExploded, score, runChoice);
								if (!isInvalidFile) //There are more valid files
								{
									m.setIsWinning(); //Resets mario's to not winning anymore
									m.resetWithHammer();
									if (board.isDonkeyExist()) {
										for (int i = 0; i < BARRELS_ARR_SIZE; i++)
											barrels[i].setEnemy();
										barrels[FIRST].setIsMoving(true);
									}
									for (auto& ghost : board.getGhosts()) {
										ghost->setEnemy();
									}
									isPaused = false;
									lastBarrelExploded = false;
									countingBarrelsOnBoard = FIRST;
									continue;
								}
								else //The file isn't valid
								{
									if (currentFileIndex != fileNames.size())
									{
										m.setIsWinning();
										if ((currentFileIndex + 1) == fileNames.size()) //Reached to the last file
										{
											board.resetToWinning(); //Prints you won message
											board.printInMiddle();
											backToMainMenu(board);
											m.setLives(); //Resets mario's lives
											countingBarrelsOnBoard = FIRST;
											score = FIRST;
											return;
										}
									}
									m.resetWithHammer();
									break;
								}
							}
						}
						m.draw(m.getChar(), m.getColorChoice(), runChoice);
						collision(board.getGhosts());
						for (auto& ghost : board.getGhosts()) {
							ghost->draw(ghost->getChar(), ghost->getColorChoice(), runChoice);
							ghost->keyByFloor();
						}
						if (board.isDonkeyExist())
						{
							for (int i = 0; i < BARRELS_ARR_SIZE; i++)
							{
								if (barrels[i].getIsMoving())
								{
									barrels[i].draw(barrels[i].getChar(), barrels[i].getColorChoice(), runChoice);
								}
							}
						}
						if (_kbhit()) {
							char key = _getch();
							if (key == ESC) //The game is paused, using chat GPT
							{
								if (isPaused) //If it's already paused, just need to continue
								{
									isPaused = false;
								}
								else
								{
									isPaused = true;
									board.resetToPausing();
									board.printInMiddle();
									while (_kbhit())
										_getch();
									while (isPaused)
									{
										if (_kbhit())
										{
											char resumeKey = _getch();
											if (resumeKey == ESC)
												isPaused = false;
										}
										Sleep(100);
									}
									board.reset();
									board.printFromFile();
									board.drawPauline(runChoice);
									board.drawDonkey(runChoice);
									if ((board.getHammerChar() != '\n') && (!m.getWithHammer()))
										board.drawHammer(board.getHammerChar(), runChoice);
									m.drawHearts(board.getLX(), board.getLY(), runChoice);
									drawScore(board.getIsSilent(), board.getLX(), board.getLY(), score, runChoice);
								}
							}
							else if ((std::tolower(key) == (char)GameConfig::eKeys::HAMMER) && (m.getWithHammer()))
							{
								index = findEnemy(m.getX() + m.getDirX(), m.getY() + m.getDirY(), board.getGhosts(), barrels, &enemyIndex);
								if (index != -1)
								{
									if (enemyIndex == 1)
									{
										barrels[index].Enemy::explode();
										barrels[index].exploded();
										barrels[index].resetSteps();
										barrels[index].setIsMoving(false);
									}
									else if (enemyIndex == 2)
									{
										board.getGhosts()[index]->explode();
										board.getGhosts().erase(board.getGhosts().begin() + index);
									}
									score += GHOST_SCORE;
								}
							}
							else
								m.keyPressed(key);
						}
						Sleep(100);
						if (board.isDonkeyExist())
						{
							for (int i = 0; i < BARRELS_ARR_SIZE; i++) //Counts the barrels on board
							{
								if (barrels[i].getIsExploded())
								{
									lastBarrelExploded = true;
									countingBarrelsOnBoard--;
									barrels[i].resetIsExploded();
								}
								if (barrels[i].getIsMoving())
								{
									barrels[i].draw(barrels[i].getChar(), barrels[i].getColorChoice(), runChoice);
									barrels[i].keyByFloor();
									barrels[i].eraseOnBoard(runChoice);
									barrels[i].move();
									if(barrels[i].getCountingSteps() == FIRST)
										countingBarrelsOnBoard++;
								}
								if (barrels[i].getCountingSteps() == MAX_STEPS)
								{
									barrels[(i + 1) % BARRELS_ARR_SIZE].setIsMoving(true);
								}
								else if ((lastBarrelExploded) && (countingBarrelsOnBoard == FIRST))
								{
									barrels[(i + 1) % BARRELS_ARR_SIZE].setIsMoving(true);
									lastBarrelExploded = false;
								}
							}
						}
						for (auto& ghost : board.getGhosts()) {
							ghost->eraseOnBoard(runChoice);
							ghost->move();
						}
						m.eraseOnBoard(runChoice);
						m.move();
					}
				}
			}
}


