#include "manualGame.h"

void ManualGame::run(Board& board, int choice) //Run the game on save mode
{
		int index, enemyIndex, runChoice;
		static int score = FIRST;
		std::srand(std::time(0)); // Initializes the random number generator with the current time to ensure unique random numbers
		bool isInvalidFile = false, isPaused = false, lastBarrelExploded = false, firstRun = true;
		static int countingBarrelsOnBoard = FIRST;  //Initializes the barrel count
		std::vector<std::string> fileNames;
		Steps steps;
		Results results;
		isInvalidFile = !getAllBoardFileNames(fileNames, board);
		if (isInvalidFile) //No valid files
		{
			backToMainMenu(board);
			return;
		}
		size_t currentFileIndex = FIRST;
		if (choice == SHOW_FILES)
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
			std::string filename_prefix = filename.substr(FIRST, filename.find_last_of('.'));
			std::string stepsFilename = filename_prefix + ".steps";
			std::string resultsFilename = filename_prefix + ".result";
			board.setMario(m);
			isInvalidFile = nextLevel(board, m, barrels, filename, isPaused, lastBarrelExploded, score, runChoice);
			long random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
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
			//We regenerate the seed for each screen so we can save it as part of the screen steps
			random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
			steps.setRandomSeed(random_seed);
			srand(random_seed);
			size_t iteration = FIRST;
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
				countingBarrelsOnBoard = FIRST; //Initializes the barrel count
				while (true)
				{
					iteration++;
					size_t nextCrashIteration = FIRST;
					if (m.getIsDead()) //Mario died
					{
						score = score - GHOST_SCORE;
						// save crash to the results
						results.addResult(iteration, Results::crash);
						if (m.getLives() == NO_LIVES) //Mario has no lives left
						{
							steps.saveSteps(stepsFilename);
							results.saveResults(resultsFilename);
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
						results.addResult(iteration, Results::finished);
						results.addResult(score, Results::score);
						steps.saveSteps(stepsFilename);
						results.saveResults(resultsFilename);
						score += NEXT_LEVEL_SCORE;
						iteration = FIRST;
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
							isInvalidFile = nextLevel(board, m, barrels, fileNames[currentFileIndex], isPaused, lastBarrelExploded, score, runChoice);
							filename_prefix = fileNames[currentFileIndex].substr(FIRST, fileNames[currentFileIndex].find_last_of('.')); //Load the next steps and result files
							stepsFilename = filename_prefix + ".steps";
							resultsFilename = filename_prefix + ".result";
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
								drawScore(false, board.getLX(), board.getLY(), score, runChoice); //Flase for ignoring silent mode 
							}
						}
						else if (board.keyPressed(key)) 
						{
							steps.addStep(iteration, key);
						}
						if ((std::tolower(key) == (char)GameConfig::eKeys::HAMMER) && (m.getWithHammer()))
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
								if (barrels[i].getCountingSteps() == FIRST)
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
				//Updates result and steps files
				results.addResult(iteration, Results::finished);
				steps.saveSteps(stepsFilename);
				results.saveResults(resultsFilename);
			}
			iteration = FIRST; //Initialize iteration number
		}
}
