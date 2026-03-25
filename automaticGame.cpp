#include "automaticGame.h"

void AutomaticGame::run(Board& board, int choice)
{
	int index, enemyIndex;
	char key = '\n';
	static int score = FIRST;
	std::srand(std::time(0)); // Initializes the random number generator with the current time to ensure unique random numbers
	bool isInvalidFile = false, isPaused = false, lastBarrelExploded = false, firstRun = true, isValid = true;
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
	for (; currentFileIndex < fileNames.size(); currentFileIndex++)
	{
		const auto& filename = fileNames[currentFileIndex];
		std::string filename_prefix = filename.substr(0, filename.find_last_of('.'));
		std::string stepsFilename = filename_prefix + ".steps";
		std::string resultsFilename = filename_prefix + ".result";
		board.setMario(m);
		isInvalidFile = nextLevel(board, m, barrels, filename, isPaused, lastBarrelExploded, score, choice);
		long random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
		size_t iteration = FIRST;
		if (isInvalidFile && firstRun && ((currentFileIndex + 1) == fileNames.size())) //Checks if there are no more valid files
		{
			gotoxy(MESSAGE_X, MESSAGE_Y - 1);
			std::cout << "No more valid files" << '\n';
			backToMainMenu(board);
			return;
		}
		firstRun = false;
		steps = Steps::loadSteps(stepsFilename,board,&isValid);
		if (!isValid) //Cant open steps file
		{
			reportResultError("Error: Failed to open file", fileNames[currentFileIndex], iteration, board);
			unmatching_result_found = true;
			score += NEXT_LEVEL_SCORE;
			break;
		}
		random_seed = steps.getRandomSeed();
		results = Results::loadResults(resultsFilename,&isValid); 
		if (!isValid) //Cant open result file
		{
			reportResultError("Error: Failed to open file", fileNames[currentFileIndex], iteration, board);
			unmatching_result_found = true;
			score += NEXT_LEVEL_SCORE;
			break;
		}
		srand(random_seed);
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
				iteration++;
				size_t nextCrashIteration = FIRST; 
				if (m.getIsDead()) //Mario died
				{
					score = score - GHOST_SCORE;
					if (results.popResult() != std::pair{ iteration, Results::crash }) { //Check if the result is correct
						reportResultError("Results file doesn't match crashed event!", fileNames[currentFileIndex], iteration, board);
						unmatching_result_found = true;
						break;
					}
					nextCrashIteration = results.getNextCrashIteration();
					if (m.getLives() == NO_LIVES) //Mario has no lives left
					{
						board.resetToGameOver(); //Shows game over message
						board.print();
						if (board.getIsSilent()&&!unmatching_result_found) //Prints test passed on silent mode
						{
							board.resetToTestPassedBoard();
							board.print(true);
						}
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
						isInvalidFile = nextLevel(board, m, barrels, fileNames[currentFileIndex], isPaused, lastBarrelExploded, score, choice); //We already know that this file is valid
						for (auto& ghost : board.getGhosts()) {
							ghost->setEnemy();
						}
						m.setStartPos(m.getStartX(), m.getStartY());
						countingBarrelsOnBoard = FIRST;
						m.setIsDead();
					}
				}
				//We didn't crash, so we check if the result is correct
				if (iteration == nextCrashIteration && iteration>FIRST) {
					reportResultError("Results file has crashed event that didn't happen!", fileNames[currentFileIndex], iteration, board);
					unmatching_result_found = true;
					break;
				}
				if (m.getIsWinning()) //Mario won
				{
					if (results.popResult() != std::pair{ iteration, Results::finished }) { //Check if the winning is correct
						reportResultError("Results file doesn't match winning event!", fileNames[currentFileIndex], iteration, board);
						unmatching_result_found = true;
						score += NEXT_LEVEL_SCORE;
						m.setIsWinning(); //Resets mario's to not winning anymore
						if ((currentFileIndex + 1) >= fileNames.size()) //No more files left
						{
							gotoxy(MESSAGE_X, MESSAGE_Y - 1);
							std::cout << "No files left!" << std::endl;
							backToMainMenu(board, board.getIsSilent());
							return;
						}
						break;
					} 
					if (results.popResult() != std::pair{ score, Results::score }) { //Check if the score is correct
						reportResultError("Results file doesn't match score event!", fileNames[currentFileIndex], score, board);
						unmatching_result_found = true;
						m.setIsWinning(); //Resets mario's to not winning anymore
						break;
					}
					currentFileIndex++;
					score += NEXT_LEVEL_SCORE;
					if (currentFileIndex >= fileNames.size()) //No more files left
					{
						m.setIsWinning(); //Resets mario's to not winning anymore
						board.resetToWinning(); //Prints you won message
						board.printInMiddle();
						if (board.getIsSilent()&&!unmatching_result_found) //Prints test passed on silent mode
						{
							board.resetToTestPassedBoard();
							board.print(true);
						}
						backToMainMenu(board, board.getIsSilent());
						m.setLives(); //Resets mario's lives
						countingBarrelsOnBoard = FIRST;
						score = FIRST;
						return;
					}
					else //There are more files
					{
						isInvalidFile = nextLevel(board, m, barrels, fileNames[currentFileIndex], isPaused, lastBarrelExploded, score, choice);
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
							filename_prefix = fileNames[currentFileIndex].substr(FIRST, fileNames[currentFileIndex].find_last_of('.')); //Loading the next file steps and result
							stepsFilename = filename_prefix + ".steps";
							resultsFilename = filename_prefix + ".result";
							steps = Steps::loadSteps(stepsFilename, board, &isValid);
							if (!isValid) //Cant open steps file
							{
								reportResultError("Error: Failed to open file", fileNames[currentFileIndex], iteration, board);
								unmatching_result_found = true;
								score += NEXT_LEVEL_SCORE;
								break;
							}
							random_seed = steps.getRandomSeed();
							results = Results::loadResults(resultsFilename, &isValid);
							if (!isValid) //Cant open result file
							{
								reportResultError("Error: Failed to open file", fileNames[currentFileIndex], iteration, board);
								unmatching_result_found = true;
								score += NEXT_LEVEL_SCORE;
								break;
							}
							srand(random_seed);
							iteration = FIRST;
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
				if (results.isFinishedBy(iteration)) { //Checks if the iteration matches
					reportResultError("Results file reached finish while game hadn't!", fileNames[currentFileIndex], iteration, board);
					unmatching_result_found = true;
					break;
				}
				else {
					nextCrashIteration = results.getNextCrashIteration();
				}
				m.draw(m.getChar(), m.getColorChoice(), choice);
				collision(board.getGhosts());
				for (auto& ghost : board.getGhosts()) {
					ghost->draw(ghost->getChar(), ghost->getColorChoice(), choice);
					ghost->keyByFloor();
				}
				if (board.isDonkeyExist())
				{
					for (int i = 0; i < BARRELS_ARR_SIZE; i++)
					{
						if (barrels[i].getIsMoving())
						{
							barrels[i].draw(barrels[i].getChar(), barrels[i].getColorChoice(), choice);
						}
					}
				}
				if (steps.isNextStepOnIteration(iteration)) 
				{
					key = steps.popStep();
					board.keyPressed(key);
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
					Sleep(board.getIsSilent() ? 0 : 50 );
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
							barrels[i].draw(barrels[i].getChar(), barrels[i].getColorChoice());
							barrels[i].keyByFloor();
							barrels[i].eraseOnBoard();
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
					ghost->eraseOnBoard();
					ghost->move();
				}
				m.eraseOnBoard();
				m.move();
				
			}
			// check if the result is correct, but only if we didn't find an unmatching result already
			if (!unmatching_result_found) {
				if ((results.popResult() != std::pair{ iteration, Results::finished })&&(isValid)) {
					reportResultError("Results file doesn't match finished event!", fileNames[currentFileIndex], iteration, board);
					unmatching_result_found = true;
				}
				if ((results.popResult().second != Results::noResult)&&(isValid)) {
					reportResultError("Results file has additional events after finish event!", fileNames[currentFileIndex], iteration, board);
					unmatching_result_found = true;
				}
			}
			iteration = FIRST; //Initialize iteration number
		}
	}

}

void AutomaticGame::backToMainMenu(Board& board, bool printInSilent) //Prints exiting and game over message
{
	int y = MESSAGE_Y;
	if (!board.getIsSilent())
		y -=5;
	if (!board.getIsSilent() || printInSilent)
	{
		for (int i = 5; i >= 1; i--) //Shows back to main menu message with count
		{
			gotoxy(MESSAGE_X, y);
			std::cout << "Exiting in: " << i;
			Sleep(1000);
		}
	}
	board.resetToGameOver();
	board.print(board.getIsSilent());
}
