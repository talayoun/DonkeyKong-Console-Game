#include "ghost.h"
#include "theGame.h"
#include <cstdlib>
#include <ctime>

void Ghost::keyByFloor()  //Sets the ghost's direction according to the floor
{
	int probability;
	probability = std::rand() % 100 + 1;
	if (probability <= 95) //Default direction
	{
		if(!isMoving)
		{
			isMovingRight = true;
			isMovingLeft = false;
		}
	}
	else //Change direction
	{
		if (isMovingRight)
		{
			isMovingRight = false;
			isMovingLeft = true;
		}
		else
		{
			isMovingRight = true;
			isMovingLeft = false;
		}
		
	}
	isMoving = true;
}

void Ghost::move()
{
	Board b;

	if (isMovingLeft)
	{
		if (!isFloor(x - 1, y)) //No floor on next step, change dir
		{
			isMovingLeft = false;
			isMovingRight = true;
			dir.x = 1;
		}
		else
		{
			dir.x = -1;
		}
	}

	if (isMovingRight) //No floor on next step, change dir
	{
		if (!isFloor(x + 1, y))
		{
			isMovingLeft = true;
			isMovingRight = false;
			dir.x = -1;
		}
		else
		{
			dir.x = 1;
		}
	}

	if (crashingWithMario()) //The ghost is crashing with mario
	{
		pMario->marioDied(); //Mario loses live
	}
	int newX = x + dir.x;
	int newY = y + dir.y;

	if ((pBoard->getChar(newX, newY) == b.getBorder()) || ((isFloor(x, y + 1)) && (pBoard->getChar(newX, y+1) == ' ')))
	{
		if (dir.x == -1) //To change direction and keep moving
		{
			dir = { 1, 0 };
			isMovingLeft = false;
			isMovingRight = true;
		}
			
		else if (dir.x == 1)
		{
			dir = { -1, 0 };
			isMovingLeft = true;
			isMovingRight = false;
		}
			
	}
	else
	{
		x = newX;
		y = newY;
	}
}


void Ghost::setEnemy() //Set ghost
{
	eraseOnBoard();
	while (!isFloor(startX, startY)) //If we get a ghost blowing in the air - we will bring it down
	{
		startY++;
	}
	setStartPos(startX, startY);
	setIsMoving(false);
}

