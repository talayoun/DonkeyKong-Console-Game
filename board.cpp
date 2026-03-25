#include <cstring>
#include <iostream>
#include <fstream>
#include "board.h"
#include "theGame.h"
#include <vector>
#include <memory>


void Board::setGhosts(std::vector<std::unique_ptr<Ghost>>&& newGhosts) {
	this->ghosts.clear();
	for (const auto& ghost : newGhosts) {
		this->ghosts.push_back(ghost->duplicate());
	}
}

bool Board::load(const std::string& filename) { //Loads and fixes the game files
	int countMarioChar = FIRST, countPaulineChar = FIRST, countDonkeyChar = FIRST, countHammerChar = FIRST,countTabs = FIRST;
	bool validC = true, isInvalidBoard = false;
	std::vector<std::unique_ptr<Ghost>> ghosts;
	std::ifstream screen_file(filename);
	if (!screen_file.is_open()) {
		std::cerr << "Couln't open the file" << filename << std::endl; 
	}
	int curr_row = FIRST;
	int curr_col = FIRST;
	char c;
	while (!screen_file.get(c).eof() && curr_row < MAX_Y) {
		if (c == '\n') {
			if (curr_col < MAX_X) {
				// Add spaces for missing cols
#pragma warning(suppress : 4996) // To allow strcpy
				strcpy(originalBoard[curr_row] + curr_col, std::string(MAX_X - curr_col - 1, ' ').c_str());
			}
			++curr_row;
			curr_col = FIRST;
			continue;
		}
		if (curr_col < MAX_X) {
			if (c == (char)GameConfig::eKeys::MARIO) {
				countMarioChar++;
				pMario->setStartPos(curr_col, curr_row);
				originalBoard[curr_row][curr_col++] = ' ';
			}
			else if (c == (char)GameConfig::eKeys::PAULINE) {
				countPaulineChar++;
				setPaulinePos(curr_col, curr_row);
				originalBoard[curr_row][curr_col++] = c;
			}
			else if (c == (char)GameConfig::eKeys::KONG) {
				countDonkeyChar++;
				setDonkeyPos(curr_col, curr_row);
				originalBoard[curr_row][curr_col++] = c;
				donkeyExist = true;
			}
			else if (c == 'p' || c == 'P') { //In case 'P' (error character) so we will automatically change it to 'p'
				countHammerChar++;
				setHammerChar((char)GameConfig::eKeys::HAMMER);
				setHammerPos(curr_col, curr_row);
				originalBoard[curr_row][curr_col++] = c;
			}
			else if (c == (char)GameConfig::eKeys::GHOST) { 
				auto ghost = std::make_unique<Ghost>(curr_col, curr_row);
				ghost->setStartPos(curr_col, curr_row);
				ghosts.push_back(std::move(ghost));
				originalBoard[curr_row][curr_col++] = ' ';
			}
			else if (c == (char)GameConfig::eKeys::ADVANCEDGHOST) {
				auto ghost = std::make_unique<AdvancedGhost>(curr_col, curr_row);
				ghost->setStartPos(curr_col, curr_row);
				ghosts.push_back(std::move(ghost));
				originalBoard[curr_row][curr_col++] = ' ';
			}
			else if (c == (char)GameConfig::eKeys::LEGEND) {
				setLPos(curr_col, curr_row);
				originalBoard[curr_row][curr_col++] = ' ';
			}
			else if (c == '\t')
				countTabs++;
			else
			{
				originalBoard[curr_row][curr_col++] = c;
				if (c != ' ' && c != '=' && c != '<' && c != '>' && c != 'Q' && c != 'L' && c != '3' && c != '\3' && c != 'H' && c != 'O' && c!='^')
				{
					validC = false;
				}
			}

		}
	}
	this->setGhosts(std::move(ghosts));
	isInvalidBoard = invalidBoard(countMarioChar, countPaulineChar, countDonkeyChar, countHammerChar, validC, countTabs);
	if (curr_row < MAX_Y)
	{
		for (int row = curr_row; row < MAX_Y; ++row) {
			#pragma warning(suppress : 4996) // To allow strcpy
			strcpy(originalBoard[row], std::string(MAX_X, 'Q').c_str());
			originalBoard[row][MAX_X] = '\n';
		}
		curr_row = MAX_Y;
	}
	else if (curr_row > MAX_Y) {
		curr_row = MAX_Y - 1;
	}
	int last_row = (curr_row < MAX_Y ? curr_row : MAX_Y - 1);
	#pragma warning(suppress : 4996) // To allow strcpy
	strcpy(originalBoard[0], std::string(MAX_X, 'Q').c_str());
	originalBoard[0][MAX_X] = '\n';

	#pragma warning(suppress : 4996) // To allow strcpy
	strcpy(originalBoard[last_row], std::string(MAX_X, 'Q').c_str());
	originalBoard[last_row][MAX_X] = '\0';

	for (int row = 1; row < last_row; ++row) {
		originalBoard[row][0] = 'Q';          
		originalBoard[row][MAX_X - 1] = 'Q';  
		originalBoard[row][MAX_X] = '\n';
	}
	return isInvalidBoard;
}

//Checks if the board is valid, if not prints a proper message
bool Board::invalidBoard(int countMarioChar, int countPaulineChar, int countDonkeyChar, int countHammerChar, bool validChar, int countTabs)
{
	bool res = false;
	std::vector<std::string> messages;
	if (countMarioChar > 1)
	{
		messages.push_back("Too many Marios");
		res = true;
	}
	else if (countMarioChar == 0)
	{
		messages.push_back("Mario was not found in range");
		res = true;
	}
	if (countPaulineChar > 1)
	{
		messages.push_back("Too many Paulines");
		res = true;
	}
	else if (countPaulineChar == 0)
	{
		messages.push_back("Pauline was not found in range");
		res = true;
	}
	if (countDonkeyChar > 1)
	{
		messages.push_back("Too many Donkeys");
		res = true;
	}
	else if (countDonkeyChar == 0)
	{
		noDonkey();
	}
	if (countTabs > 0)
	{
		messages.push_back("No Tabs Allowed");
		res = true;
	}
	if (countHammerChar > 1)
	{
		messages.push_back("Too many Hammers");
		res = true;
	}
	if (!validChar)
	{
		messages.push_back("Invalid char was found");
		res = true;
	}
	if (res)
	{
		int messageY = 9;
		resetToInvalidFile();
		print();
		for (const auto& msg : messages)
		{
			gotoxy(26, messageY);
			std::cout << msg;
			messageY++;
		}
		Sleep(5000);
	}
	return res;
}


void Board::reset() {
	// Copy the original board to the current board, as an entire 2d array
	memcpy(currentBoard, originalBoard, MAX_Y * (MAX_X + 1));
}

void Board::print(bool printInSilent) const //Prints the board
{
	if ((!isSilent) || (printInSilent)) {
		system("cls");
		gotoxy(0, 0);
		for (int i = 0; i < MAX_Y - 1; i++) {
			std::cout << currentBoard[i] << '\n';
		}
		std::cout << currentBoard[MAX_Y - 1];
	}
}
void Board::printFromFile() const
{
	if (!isSilent) {
		system("cls");
		std::cout << currentBoard[FIRST];
	}
}

void Board::printInMiddle() const //Prints in the middle
{
	if (!isSilent) {
		gotoxy(0, 6);
		for (int i = 0; i < MAX_Y_SHORT_BOARD - 1; i++) {
			std::cout << currentBoard[i] << '\n';
		}
		std::cout << currentBoard[MAX_Y_SHORT_BOARD - 1];
	}
}

void Board::resetToStartGame() //Resets to starting board
{
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], startGameBoard[i], MAX_X + 1);
	}
}

void Board::resetToGameOver() { //Resets to game over message
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], gameOverBoard[i], MAX_X + 1);
	}
}

void Board::resetToInstructions() { //Resets to instructions menu
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], instructionsBoard[i], MAX_X + 1);
	}
}

void Board::resetToWinning() {  //Resets to you won message
	for (int i = 0; i < MAX_Y_SHORT_BOARD; i++) {
		memcpy(currentBoard[i], winningBoard[i], MAX_X + 1);
	}
}

void Board::resetToPausing() {  //Resets to paused message
	for (int i = 0; i < MAX_Y_SHORT_BOARD; i++) {
		memcpy(currentBoard[i], pausingBoard[i], MAX_X + 1);
	}
}

void Board::resetToInvalidFile() { //Resets to invalid message
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], invalidFileBoard[i], MAX_X + 1);
	}
}
void Board::resetTochooseAFileBoard() { //Resets to choose a file message
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], chooseAFileBoard[i], MAX_X + 1);
	}
}

void Board::resetToTestPassedBoard() { //Resets to test passed message
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], testPassedBoard[i], MAX_X + 1);
	}
}

void Board::resetToTestFailedBoard() { //Resets to test failed message
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], testFailedBoard[i], MAX_X + 1);
	}
}

void Board::drawPauline(int choice) const //Draw pauline in her spot
{
	if (!isSilent) {
		if (choice == COLOR) //Draw with color
		{
			gotoxy(getPaulineX(), getPaulineY());
			GameConfig g;
			g.setColor(FOREGROUND_RED | FOREGROUND_BLUE);
			std::cout << ('$');
			g.setColor(g.BASIC);
		}
		else if (choice == NO_COLOR) //Draw without color
		{
			gotoxy(getPaulineX(), getPaulineY());
			std::cout << ('$');
		}
	}
}

void Board::drawDonkey(int choice) const  //Draw donkey in his spot
{
	if (!isSilent) {
		if (isDonkeyExist())
		{
			if (choice == COLOR)  //Draw with color
			{
				gotoxy(getDonkeyX(), getDonkeyY());
				GameConfig g;
				g.setColor(FOREGROUND_BLUE | FOREGROUND_GREEN);
				std::cout << ('&');
				g.setColor(g.BASIC);
			}
			else if (choice == NO_COLOR)  //Draw without color
			{
				gotoxy(getDonkeyX(), getDonkeyY());
				std::cout << ('&');
			}
		}
	}
}

void Board::drawHammer(char c, int choice) const //Draw hammer in spot
{
	if (!isSilent) {
		if (choice == COLOR) //Draw with color
		{
			gotoxy(getHammerX(), getHammerY());
			GameConfig g;
			g.setColor(g.GREEN);
			std::cout << (c);
			g.setColor(g.BASIC);
		}
		else if (choice == NO_COLOR) //Draw without color
		{
			gotoxy(getHammerX(), getHammerY());
			std::cout << (c);
		}
	}
}

bool Board::keyPressed(char key) {
	return pMario->keyPressed(key);
}
