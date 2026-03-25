#include <iostream>
#include <windows.h>
#include <conio.h>
#include "Board.h"
#include "general.h"
#include "mario.h"
#include "barrel.h"
#include "theGame.h"
#include "automaticGame.h"
#include "manualGame.h"

int main(int argc, char** argv)
{
	ShowConsoleCursor(false);
	Board board;
	bool isLoad = argc > LOAD_SAVE_INDEX && std::string(argv[LOAD_SAVE_INDEX]) == "-load";
	bool isSave = argc > LOAD_SAVE_INDEX && std::string(argv[LOAD_SAVE_INDEX]) == "-save";
	bool isSilent = isLoad && argc > SILENT_INDEX && std::string(argv[SILENT_INDEX]) == "-silent";
	if(!isLoad)
	{
		board.resetToStartGame();
		board.print();
	}
	board.setIsSilent(isSilent);
	TheGame* game;
	if (isLoad)
	{
		game = new AutomaticGame();  //Creating automatic game, loading from file
		game->run(board);
	}
	else if (isSave)
		game = new ManualGame();  //Creating manual game, saving to file
	else
		game = new TheGame();
	while (true&&!isLoad)
	{
		if (_kbhit()) {
			char choice = _getch();
			switch (choice) {
			case RUN_WITHOUT_COLOR: //Start a new game without colors
				game->run(board);
				break;
			case RUN_WITH_COLOR: //Start a new game with colors
				game->run(board, COLOR);
				break;
			case SHOW_GAME_FILES: //Choose a file to start the game with
				board.resetTochooseAFileBoard();
				board.print();
				game->run(board, SHOW_FILES);
				break;
			case SHOW_INSTRUCTIONS: //Show instructions
				board.resetToInstructions();
				board.print();
				_getch(); // Wait for a key press
				board.resetToStartGame(); //Reset to the start screen
				board.print();
				break;
			case EXIT: //Exit the game
				board.resetToGameOver();
				board.print();
				return 0;
			}
		}
	}
	delete game;
}