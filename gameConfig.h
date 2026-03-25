#pragma once
#include <Windows.h>

class GameConfig //Using Amir Kirsh's code
{
public:
	enum class eKeys { ESC = 27, UP = 'w', DOWN = 'x', LEFT = 'a', RIGHT = 'd', STAY = 's', WALL = 'Q', LADDER = 'H', GROUND_N = '=', GROUND_L = '<', GROUND_R = '>', PAULINE = '$', KONG = '&', BARREL = 'O', HAMMER = 'p', GHOST = 'x', ADVANCEDGHOST = 'X',  MARIO_WITH_HAMMER = '%', MARIO = '@', LEGEND = 'L' };
	static const char signs[];

	static constexpr char EMPTY_TILE = ' ';

	static constexpr int GAME_WIDTH = 80;
	static constexpr int GAME_HEIGHT = 25;

	//Setting some colors
	static constexpr int FOREGROUND_BLACK = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
	static constexpr int BACKGROUND_BLACK = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN;

	static constexpr int BASIC = 7; //Basic color
	static constexpr int YELLOW = FOREGROUND_RED | FOREGROUND_GREEN; //Yellow color
	static constexpr int GREY = 8;
	static constexpr int GREEN = 2;
	static constexpr int RED = FOREGROUND_RED;
	static constexpr int PINK = FOREGROUND_RED | FOREGROUND_BLUE;
	static constexpr int LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_GREEN;

	void setColor(int color) const //Receives color code and setes the text to this color
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdOut, color);
	}
};


