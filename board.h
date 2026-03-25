#pragma once

#include "general.h"
#include "gameConfig.h"
#include <string>
#include <vector>
#include <memory>

constexpr int ESC = 27;
//constexpr int START_GAME = 1;
constexpr int SHOW_FILES = 3;
//constexpr int SHOW_INSTRUCTIONS = 8;
//constexpr int EXIT_GAME = 9;
constexpr char BORDER = 'Q';
class Mario;
class Ghost;
class AdvancedGhost;

class Board
{ //using Amir Kirsh's code
	static constexpr int MAX_X = 80;
	static constexpr int MAX_Y = 25;
	static constexpr int MAX_Y_SHORT_BOARD = 13;
	char originalBoard[MAX_Y][MAX_X + 1];
	char currentBoard[MAX_Y][MAX_X + 1]; // +1 for null terminator
	const char* gameOverBoard[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q                   ______    ______   __       __  ________	               Q", // 2
		  "Q                  /      \\  /      \\ |  \\     /  \\|        \\	               Q", // 3
		  "Q                 |  $$$$$$\\|  $$$$$$\\| $$\\   /  $$| $$$$$$$$                  Q", // 4  
		  "Q                 | $$ __\\$$| $$__| $$| $$$\\ /  $$$| $$__                      Q", // 5
		  "Q                 | $$|    \\| $$    $$| $$$$\\  $$$$| $$  \\                     Q", // 6
		  "Q                 | $$ \\$$$$| $$$$$$$$| $$\\$$ $$ $$| $$$$$                     Q", // 7
		  "Q                 | $$__| $$| $$  | $$| $$ \\$$$| $$| $$_____                   Q", // 8
		  "Q                  \\$$    $$| $$  | $$| $$  \\$ | $$| $$     \\                  Q", // 9
		  "Q                   \\$$$$$$  \\$$   \\$$ \\$$      \\$$ \\$$$$$$$$                  Q", // 10                                                                       
		  "Q                                                                              Q", // 11
		  "Q                    ______   __     __   ________  ______                     Q", // 12
		  "Q                   /      \\ |  \\   |  \\|        \\|       \\                    Q", // 13
		  "Q                  |  $$$$$$\\| $$   | $$| $$$$$$$$| $$$$$$$\\                   Q", // 14
		  "Q                  | $$  | $$| $$   | $$| $$__    | $$__| $$                   Q", // 15
		  "Q                  | $$  | $$ \\$$\\ /  $$| $$  \\   | $$    $$                   Q", // 16
		  "Q                  | $$  | $$  \\$$\\  $$ | $$$$$   | $$$$$$$\\                   Q", // 17
		  "Q                  | $$__/ $$   \\$$ $$  | $$_____ | $$  | $$                   Q", // 18
		  "Q                   \\$$    $$    \\$$$   | $$     \\| $$  | $$                   Q", // 19
		  "Q                    \\$$$$$$      \\$     \\$$$$$$$$ \\$$   \\$$                   Q", // 20
		  "Q                                                                              Q", // 21
		  "Q                                                                              Q", // 22
		  "Q                                                                              Q", // 23
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};
	const char* startGameBoard[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q           ____              _                _  __                           Q", // 2
		  "Q          |  _ \\  ___  _ __ | | _____ _   _  | |/ /___  _ __   __ _           Q", // 3
		  "Q          | | | |/ _ \\| '_ \\| |/ / _ \\ | | | | ' // _ \\| '_ \\ / _` |          Q", // 4
		  "Q          | |_| | (_) | | | |   <  __/ |_| | | . \\ (_) | | | | (_| |          Q", // 5
		  "Q          |____/ \\___/|_| |_|_|\\_\___| \\__, | |_|\\_ \___/|_| |_|\\__, |          Q", // 6
		  "Q                                      |___/                   |___/           Q", // 7
		  "Q                                                                              Q", // 8
		  "Q                                                                              Q", // 9
		  "Q                           __  __                                             Q", // 10
		  "Q                          |  \\/  |                 _                          Q", // 11
		  "Q                          | \\  / | ___ _ __  _   _(_)                         Q", // 12
		  "Q                          | |\\/| |/ _ \\ '_ \\| | | |                           Q", // 13
		  "Q                          | |  | |  __/ | | | |_| |_                          Q", // 14
		  "Q                          |_|  |_|\\___|_| |_|\\__,_(_)                         Q", // 15
		  "Q                                                                              Q", // 16
		  "Q                                                                              Q", // 17
		  "Q                           (1) Start a new game                               Q", // 18
		  "Q                           (2) Start a new game with colors                   Q", // 19
		  "Q                           (3) Choose a file                                  Q", // 20
		  "Q                           (8) Instructions and Keys                          Q", // 21
		  "Q                           (9) Exit                                           Q", // 22
		  "Q                                                                              Q", // 23
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};
	const char* instructionsBoard[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                 *Press any key to return to the main menu*                   Q", // 1
		  "Q           _____           _                   _   _                          Q", // 2
		  "Q          |_   _|         | |                 | | (_)                         Q", // 3
		  "Q            | |  _ __  ___| |_ _ __ _   _  ___| |_ _  ___  _ __  ___          Q", // 4
		  "Q            | | | '_ \\/ __| __| '__| | | |/ __| __| |/ _ \\| '_ \\/ __|         Q", // 5
		  "Q           _| |_| | | \\__ \\ |_| |  | |_| | (__| |_| | (_) | | | \\__ \\         Q", // 6
		  "Q          |_____|_| |_|___/\\__|_|   \\__,_|\\___|\\__|_|\\___/|_| |_|___/         Q", // 7
		  "Q                                  _    _  __                                  Q", // 9
		  "Q                                 | |  | |/ /                _                 Q", // 10
		  "Q                   __ _ _ __   __| |  | ' / ___ _   _ ___  (_)                Q", // 11
		  "Q                  / _` | '_ \\ / _` |  |  < / _ \\ | | / __|                    Q", // 12
		  "Q                 | (_| | | | | (_| |  | . \\  __/ |_| \\__ \\  _                 Q", // 13
		  "Q                  \\__,_|_| |_|\\__,_|  |_|\\_\\___|\\__, |___/ (_)                Q", // 14
		  "Q                                                 __/ |                        Q", // 15
		  "Q                                                |___/                         Q", // 16
		  "Q  The Goal: Reach Pauline ($) while avoiding barrels and ghosts.              Q", // 17
		  "Q  Gameplay: Use the arrow keys to move Mario and avoid the barrels and ghosts,Q", // 18
		  "Q            The game ends when you lose all your lives.                       Q", // 19
		  "Q            You can lose a life if a barrel explodes near you if you fall,    Q", // 20
		  "Q            or if mario crashes with a ghost.                                 Q", // 21
		  "Q            Advanced ghost (marked with 'X') can climb ladders and hunt mario.Q", // 21
		  "Q  Game Keys:LEFT (A),RIGHT (D),UP / JUMP (W),DOWN (X),STAY (S),KILL (P)       Q", // 22
		  "Q            *You can use uppercase or lowercase letters                       Q", // 23
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};
	const char* winningBoard[MAX_Y_SHORT_BOARD] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q   /$$     /$$                       /$$      /$$                     /$$     Q", // 2
		  "Q  |  $$   /$$/                      | $$  /$ | $$                    | $$     Q", // 3
		  "Q   \\  $$ /$$/$$$$$$  /$$   /$$      | $$ /$$$| $$  /$$$$$$  /$$$$$$$ | $$     Q", // 4
		  "Q    \\  $$$$/$$__  $$| $$  | $$      | $$/$$ $$ $$ /$$__  $$| $$__  $$| $$     Q", // 5
		  "Q     \\  $$/ $$  \\ $$| $$  | $$      | $$$$_  $$$$| $$  \\ $$| $$  \\ $$|__/     Q", // 6
		  "Q      | $$| $$  | $$| $$  | $$      | $$$/ \\  $$$| $$  | $$| $$  | $$         Q", // 7
		  "Q      | $$|  $$$$$$/|  $$$$$$/      | $$/   \\  $$|  $$$$$$/| $$  | $$ /$$     Q", // 8
		  "Q      |__/ \\______/  \\______/       |__/     \\__/ \\______/ |__/  |__/|__/     Q", // 9
		  "Q                                                                              Q", // 10
		  "Q                                                                              Q", // 11
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 12

	};
	const char* pausingBoard[MAX_Y_SHORT_BOARD] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q               _______ _______ __   __ _______ _______ ______                 Q", // 2
		  "Q              |       |   _   |  | |  |       |       |      |                Q", // 3
		  "Q              |    _  |  |_|  |  | |  |  _____|    ___|  _    |               Q", // 4
		  "Q              |   |_| |       |  |_|  | |_____|   |___| | |   |               Q", // 5
		  "Q              |    ___|       |       |_____  |    ___| |_|   |               Q", // 6
		  "Q              |   |   |   _   |       |_____| |   |___|       |               Q", // 7
		  "Q              |___|   |__| |__|_______|_______|_______|______|                Q", // 8
		  "Q                                                                              Q", // 9
		  "Q                        To continue press Esc again                           Q", // 10
		  "Q                                                                              Q", // 11
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 12

	};
	const char* invalidFileBoard[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q                ___                 _ _     _   _____ _ _      _              Q", // 2
		  "Q               |_ _|_ ____   ____ _| (_) __| | |  ___(_) | ___| |             Q", // 3
		  "Q                | || '_ \\ \\ / / _` | | |/ _` | | |_  | | |/ _ \\ |             Q", // 4  
		  "Q                | || | | \\ V / (_| | | | (_| | |  _| | | |  __/_|             Q", // 5
		  "Q               |___|_| |_|\\_/ \\__,_|_|_|\\__,_| |_|   |_|_|\\___(_)             Q", // 6
		  "Q                                                                              Q", // 7
		  "Q                                                                              Q", // 8
		  "Q                                                                              Q", // 9
		  "Q                                                                              Q", // 10
		  "Q                                                                              Q", // 11                                                                        
		  "Q                                                                              Q", // 12
		  "Q                                                                              Q", // 13
		  "Q                                                                              Q", // 14
		  "Q                                                                              Q", // 15
		  "Q                                                                              Q", // 16
		  "Q                                                                              Q", // 17
		  "Q                                                                              Q", // 18
		  "Q                                                                              Q", // 19
		  "Q                                                                              Q", // 20
		  "Q                                                                              Q", // 21
		  "Q                              Loading next file...                            Q", // 22
		  "Q                                                                              Q", // 23
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};
	const char* chooseAFileBoard[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q               _                                     __ _ _                   Q", // 2
		  "Q              | |                                   / _(_) |                  Q", // 3
		  "Q           ___| |__   ___   ___  ___  ___    __ _  | |_ _| | ___              Q", // 4  
		  "Q          / __| '_ \\ / _ \\ / _ \\/ __|/ _ \\  / _` | |  _| | |/ _ \\             Q", // 5
		  "Q         | (__| | | | (_) | (_) \__ \\  __/  | (_| | | | | | |  __/             Q", // 6
		  "Q          \\___|_| |_|\\___/ \___/|___/\\___|   \\__,_| |_| |_|_|\\___|             Q", // 7
		  "Q                                                                              Q", // 8
		  "Q                                                                              Q", // 9
		  "Q                                                                              Q", // 10
		  "Q                                                                              Q", // 11                                                                        
		  "Q                                                                              Q", // 12
		  "Q                                                                              Q", // 13
		  "Q                                                                              Q", // 14
		  "Q                                                                              Q", // 15
		  "Q                                                                              Q", // 16
		  "Q                                                                              Q", // 17
		  "Q                                                                              Q", // 18
		  "Q                                                                              Q", // 19
		  "Q                                                                              Q", // 20
		  "Q                                                                              Q", // 21
		  "Q                                                                              Q", // 22
		  "Q                                                                              Q", // 23
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};
	const char* testPassedBoard[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q                                                                              Q", // 2
		  "Q                                                                              Q", // 3
		  "Q                                                                              Q", // 4  
		  "Q                                                                              Q", // 5
		  "Q                                                                              Q", // 6
		  "Q                                                                              Q", // 7
		  "Q                                                                              Q", // 8
		  "Q  ______    ___  _____ ______      ____   ____  _____ _____   ___  ___    __  Q", // 9
		  "Q |      |  /  _]/ ___/|      |    |    \\ /    |/ ___// ___/  /  _]|   \\  |  | Q", // 10
		  "Q |      | /  [_(   \\_ |      |    |  o  )  o  (   \\_(   \\_  /  [_ |    \\ |  | Q", // 11                                                                        
		  "Q |_|  |_||    _]\\__  ||_|  |_|    |   _/|     |\\__  |\\__  ||    _]|  D  ||__| Q", // 12
		  "Q   |  |  |   [_ /  \\ |  |  |      |  |  |  _  |/  \\ |/  \\ ||   [_ |     | __  Q", // 13
		  "Q   |  |  |     |\\    |  |  |      |  |  |  |  |\\    |\\    ||     ||     ||  | Q", // 14
		  "Q   |__|  |_____| \\___|  |__|      |__|  |__|__| \\___| \\___||_____||_____||__| Q", // 15
		  "Q                                                                              Q", // 16
		  "Q                                                                              Q", // 17
		  "Q                                                                              Q", // 18
		  "Q                                                                              Q", // 19
		  "Q                                                                              Q", // 20
		  "Q                                                                              Q", // 21
		  "Q                                                                              Q", // 22
		  "Q                                                                              Q", // 23
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};
	const char* testFailedBoard[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ", // 0
		  "Q                                                                              Q", // 1
		  "Q                                                                              Q", // 2
		  "Q ______    ___  _____ ______      _____   ____  _      ____    ___  ___    __ Q", // 3
		  "Q|      |  /  _]/ ___/|      |    |     | /    || |    |    |  /  _]|   \\  |  |Q", // 4  
		  "Q|      | /  [_(   \\_ |      |    |   __||  o  || |     |  |  /  [_ |    \\ |  |Q", // 5
		  "Q|_|  |_||    _]\\__  ||_|  |_|    |  |_  |     || |___  |  | |    _]|  D  ||__|Q", // 6
		  "Q  |  |  |   [_ /  \\ |  |  |      |   _] |  _  ||     | |  | |   [_ |     | __ Q", // 7
		  "Q  |  |  |     |\\    |  |  |      |  |   |  |  ||     | |  | |     ||     ||  |Q", // 8
		  "Q  |__|  |_____| \\___|  |__|      |__|   |__|__||_____||____||_____||_____||__|Q", // 9
		  "Q                                                                              Q", // 10
		  "Q                                                                              Q", // 11                                                                        
		  "Q                                                                              Q", // 12
		  "Q                                                                              Q", // 13
		  "Q                                                                              Q", // 14
		  "Q                                                                              Q", // 15
		  "Q                                                                              Q", // 16
		  "Q                                                                              Q", // 17
		  "Q                                                                              Q", // 18
		  "Q                                                                              Q", // 19
		  "Q                                                                              Q", // 20
		  "Q                                                                              Q", // 21
		  "Q                                                                              Q", // 22
		  "Q                                                                              Q", // 23
		  "QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"  // 24
	};
	char hammerChar = '\n';
	bool donkeyExist = false;
	int paulineX, paulineY;
	int donkeyX, donkeyY;
	int hammerX, hammerY;
	int LX, LY;
	bool isSilent = false;

	Mario* pMario;
	std::vector<std::unique_ptr<Ghost>> ghosts;

public:
	bool load(const std::string& filename);
	bool invalidBoard(int countMarioChar, int countPaulineChar, int countDonkeyChar, int countHammerChar, bool validChar, int countTabs);
	void reset();
	void print(bool printInSilent = false) const;
	void printFromFile() const;
	void printInMiddle() const;
	char getChar(int x, int y) const {
		return originalBoard[y][x];
	}
	void resetToStartGame();
	void resetToGameOver();
	void resetToInstructions();
	void resetToWinning();
	void resetToPausing();
	void resetToInvalidFile();
	void resetTochooseAFileBoard();
	void resetToTestPassedBoard();
	void resetToTestFailedBoard();
	void drawPauline(int choice = 0) const;
	void drawDonkey(int choice = 0) const;
	void drawHammer(char c, int choice = 0) const;
	void setCharOnOriginalBoard(int x, int y, char c) {
		originalBoard[y][x] = c;
	}
	int getPaulineX() const {
		return paulineX;
	}
	int getPaulineY() const {
		return paulineY;
	}
	int getDonkeyX() const {
		return donkeyX;
	}
	int getDonkeyY() const {
		return donkeyY;
	}
	int getHammerX() const {
		return hammerX;
	}
	int getHammerY() const {
		return hammerY;
	}
	int getLX() const {
		return LX;
	}
	int getLY() const {
		return LY;
	}

	void setHammerChar(char c) {
		hammerChar = c;
	}
	char getHammerChar() const {
		return hammerChar;
	}

	void noDonkey() {
		donkeyExist = false;
	}

	bool isDonkeyExist()const {
		return donkeyExist;
	}

	char getBorder() const {
		return BORDER;
	}
	void setPaulinePos(int x, int y) {
		paulineX = x;
		paulineY = y;
	}
	void setDonkeyPos(int x, int y) {
		donkeyX = x;
		donkeyY = y;
	}
	void setHammerPos(int x, int y) {
		hammerX = x;
		hammerY = y;
	}

	void setLPos(int x, int y) {
		LX = x;
		LY = y;
	}

	void setMario(Mario& mario) {
		pMario = &mario;
	}
	void setGhosts(std::vector<std::unique_ptr<Ghost>>&& newGhosts);

	void setAdvancedGhosts(std::vector<std::unique_ptr<Ghost>>&& newAdvancedGhosts);

	std::vector<std::unique_ptr<Ghost>>& getGhosts() {
		return ghosts;
	}

	void setIsSilent(bool isSilentMode) {
		isSilent = isSilentMode;
	}
	bool getIsSilent() const {
		return isSilent;
	}
	bool keyPressed(char key);
};



