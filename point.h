#pragma once

#include "board.h"
#include "iostream"

#include "gameConfig.h"

class Point
{
protected:
	int x = 0, y = 0;
	int startX, startY;
	char c;
	Board* pBoard = nullptr;
	GameConfig g;
	enum { MAX_X = 80, MAX_Y = 25 };
	enum { NO_COLOR = 0, COLOR = 1 };
	struct Direction { int x, y; }; // Inner private struct

	//The directions array order is exactly the same as the keys array - must keep it that way
	static constexpr Direction directions[] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0}, {0, 0} };
	Direction dir{ 0, 0 }; //Current direction: dir.x, dir.y
	
	void draw(char c) const { //Draw entity
		if (!pBoard->getIsSilent()) {
			gotoxy(x, y);
			std::cout << c;
		}
	}
	
public:
	Point(int _x, int _y, char _c, Board* b = nullptr) : startX(_x), startY(_y), c(_c), x(_x), y(_y), pBoard(b) {}

	Point& operator=(const Point& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
			startX = other.startX;
			startY = other.startY;
			c = other.c;
		}
		return *this;
	}

	virtual int getColorChoice() const {
		return 7;
	}

	void setBoard(Board& board) {
		pBoard = &board;
	}

	void draw(char c, int color_choice, int choice = 0) const;
	void eraseOnBoard(int choice = 0 ) { //Erase char
		char c = pBoard->getChar(x, y);
		if (c == pBoard->getHammerChar())
		{
			pBoard->drawHammer(c, choice);
		}
		else if (c == (char)GameConfig::eKeys::PAULINE)
		{
			pBoard->drawPauline(choice);
		}
		else if (c == (char)GameConfig::eKeys::KONG)
		{
			pBoard->drawDonkey(choice);
		}
		else if (c == (char)GameConfig::eKeys::BARREL)
		{
			draw(c, g.YELLOW, choice);
		}
		else if (c == (char)GameConfig::eKeys::GHOST)
		{
			draw(c, g.GREY, choice);
		}
		else if (c == (char)GameConfig::eKeys::ADVANCEDGHOST)
		{
			draw(c, g.GREY, choice);
		}
		else draw(c);
	}

	int getDirX() const { //Returns direction
		if (dir.x == 1)
			return 1;
		else if (dir.x == -1)
			return -1;
		return 0;
	}

	int getDirY() const { //Returns direction
		if (dir.y == 1)
			return 1;
		else if (dir.y == -1)
			return -1;
		return 0;
	}

	void resetDir() {
		dir.x = 0;
		dir.y = 0;
	}

	int getX() const { //Return x location
		return x;
	}

	int getY() const { //Return y location
		return y;
	}

	virtual char getChar() const { //Return the object's char
		return c;
	}

	void setX(int _x) {
		x = _x;
	}
	void setY(int _y) {
		y = _y;
	}


	void setDirX(int _x) {
		dir.x = _x;
	}

	void setDirY(int _y) {
		dir.y = _y;
	} 

	virtual void setStartPos(int _x, int _y) {
		startX = _x;
		startY = _y;
		x = _x;
		y = _y;
	}

	int getStartX() const {
		return startX;
	}

	int getStartY() const {
		return startY;
	}
	virtual ~Point() {};
};

