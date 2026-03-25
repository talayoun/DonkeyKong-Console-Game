#include "advancedGhost.h"

void AdvancedGhost::keyByFloor()  //Sets the ghost's direction according to the floor
{

	if (pBoard->getChar(x, y) == 'H') //Can climb the ladder
	{
		isClimbing = true;
		isMovingRight = false;
		isMovingLeft = false;
	}
	else  
	{
		if (pMario->getX() > x) //Advanced Ghosts go after mario
		{
			isMovingRight = true;
			isMovingLeft = false;
		}
		else if (pMario->getX() < x)
		{
			isMovingRight = false;
			isMovingLeft = true;
		}
	}
	isMoving = true;
}


void AdvancedGhost::move()
{
	Board b;

	if (isClimbing)
	{
		if ((pBoard->getChar(x, y) != 'H') && (isFloor(x, y))) //AdvancedGhost got to the end of the ladder
		{
			dir.x = dir.y = 0;
			isClimbing = false;
			isMovingRight = true; //Defult direction after the ladder
		}
		else
		{
			dir.x = 0;
			dir.y = -1;
		}
	}
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

	if ((pBoard->getChar(newX, newY) == b.getBorder()) || ((isFloor(x, y + 1)) && (pBoard->getChar(newX, y + 1) == ' ')))
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
