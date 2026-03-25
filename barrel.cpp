#include "barrel.h"
#include "theGame.h"

void Barrel::keyByFloor()  //Sets the barrel's direction according to the floor
{
	dir.y = 0;
	if (pBoard->getChar(x, y + 1) == '>')
	{
		dir.x = 1;
	}
	else if (pBoard->getChar(x, y + 1) == '<')
	{
		dir.x = -1;
	}
	else if (pBoard->getChar(x, y + 1) == ' ')
	{

		isFalling = true;
	}
	else if (((pBoard->getChar(x, y + 1) == '=') || (pBoard->getChar(x, y + 1) == pBoard->getBorder())) && (countingSteps <= 1)) //Defult dir for starting on '='
	{
		dir.x = 1;
	}
	else if (pBoard->getChar(x, y + 1) == '^')
	{
		if ((dir.x == 1) && (!(pBoard->getChar(x - 1, y + 1) == '>'))) //Barrel going right direction so change to left only if won't get stuck
			dir.x = -1;
		else if ((dir.x == -1) && (!(pBoard->getChar(x - 1, y + 1) == '<')))
			dir.x = 1;
		else //Barrel doesn't have any direction yet so it will move by the next/last char
		{
			if(pBoard->getChar(x+1, y + 1) == '>')
				dir.x = 1;
			else if(pBoard->getChar(x-1, y + 1) == '<')
				dir.x = -1;
			else if ((pBoard->getChar(x+1, y + 1) == '=') || (pBoard->getChar(x+1, y + 1) == pBoard->getBorder()))
			{
				dir.x = 1;
			}
		}
	}
}

void Barrel::move()
{
	Board b;

	if (!isFloor(x, y)) //The barrel isn't on the floor
	{
		levelDown++;
		isFalling = true;
	}

	if (isFalling) //The barrel is falling
	{
		dir.y = 1; //The direction of the barrel is down

		if (isFloor(x, y))
		{
			if (levelDown >= 8) //The barrel explodes
			{
				explode();
				isExploded = true;
				isMoving = false;
			}
			isFalling = false; //The barrel isn't falling anymore
			levelDown = 0;
			countingSteps = 0;
			dir.y = 0;
		}
	}

	if (crashingWithMario()) //The barrel is crashing with mario
	{
		pMario->marioDied(); //Tario loses live
	}

	if (barrelOnFloorsEdge(x, y)) //The barrel will exploded also
	{
		explode();
		isExploded = true;
		isMoving = false;
		countingSteps = 0;
		dir.y = 0;
	}

	int newX = x + dir.x;
	int newY = y + dir.y;

	if (pBoard->getChar(newX, newY) == b.getBorder())
	{
		if (isFloor(x, y))
		{
			if (dir.x == -1) //To change direction and keep moving
				dir = { 1, 0 };
			else if (dir.x == 1)
				dir = { -1, 0 };
		}
		else
			dir = { 0, 0 };
	}
	else
	{
		x = newX;
		y = newY;
		countingSteps++; //To measure the time for the next barrel to come out
	}
}

void Barrel::explode() //The barrel is exploding 
{
	if(!pBoard->getIsSilent())
	{
		eraseOnBoard();
		gotoxy(x, y);
		std::cout << 'X';

		//Looks like an explotion
		if (pBoard->getChar(x, y - 1) == ' ') {
			gotoxy(x, y - 1);
			std::cout << '|';
			Sleep(1);
			gotoxy(x, y - 1);
			std::cout << ' ';
		}
		if (pBoard->getChar(x + 1, y - 1) == ' ') {
			gotoxy(x + 1, y - 1);
			std::cout << '/';
			Sleep(1);
			gotoxy(x + 1, y - 1);
			std::cout << ' ';
		}
		if (pBoard->getChar(x - 1, y - 1) == ' ') {
			gotoxy(x - 1, y - 1);
			std::cout << '\\';
			Sleep(1);
			gotoxy(x - 1, y - 1);
			std::cout << ' ';
		}
		if (pBoard->getChar(x - 1, y) == ' ') {
			gotoxy(x - 1, y);
			std::cout << '-';
			Sleep(1);
			gotoxy(x - 1, y);
			std::cout << ' ';
		}
		if (pBoard->getChar(x + 1, y) == ' ') {
			gotoxy(x + 1, y);
			std::cout << '-';
			Sleep(1);
			gotoxy(x + 1, y);
			std::cout << ' ';
		}
		eraseOnBoard();

		if (isMarioClose(pMario->getX(), pMario->getY())) //Checks if mario is close, and if it needs to explode
		{
			pMario->marioDied();
		}
		x = pBoard->getDonkeyX() + 1;
		y = pBoard->getDonkeyY();
	}

}

bool Barrel::barrelOnFloorsEdge(int x, int y) const //Checks if the barrel's on the border
{
	if (((x == BORDER1_X) && (y == BORDER_Y)) || ((x == BORDER2_X) && (y == BORDER_Y)))
	{
		return true;
	}
	return false;
}

bool Barrel::isMarioClose(int marioX, int marioY) const //If mario is close to the explotion
{
	if ((abs(marioX - x) <= MARIO_RADIUS) && (abs(marioY - y) <= MARIO_RADIUS))
	{
		return true;
	}
	return false;
}

void Barrel::setEnemy() //Set barrel
{
	eraseOnBoard();
	setStartPos(pBoard->getDonkeyX(), pBoard->getDonkeyY());
	resetSteps();
	setIsMoving(false);
}


