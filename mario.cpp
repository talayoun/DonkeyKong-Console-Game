#include "mario.h"
#include "gameConfig.h"
#include "theGame.h"

bool Mario::keyPressed(char key) {
	for (size_t i = 0; i < numKeys; i++) {
		if (std::tolower(key) == keys[i]) { //In case we get capital letters
			if (std::tolower(key) == (char)GameConfig::eKeys::UP)
			{
				if (pBoard->getChar(x, y) == 'H') //Can climbe the ladder
				{
					isClimbing = true;
					dir.x = 0;
					dir.y = -1;
				}
				else //Jump
				{
					if (!(canJump(x, y)))
					{
						dir.y = 0;
						isJumping = false;
					}
					else
					{
						isJumping = true;
						dir.y = -1;
					}
				}
			}
			else if (std::tolower(key) == (char)GameConfig::eKeys::DOWN)
			{
				if ((pBoard->getChar(x, y + 2) == 'H') && (isFloor(x, y))) //Can go down the ladder
				{
					isDownLadder = true;
					dir.x = 0;
					dir.y = 1;
				}
			}
			else if (std::tolower(key) == (char)GameConfig::eKeys::STAY)
			{
				dir.x = 0;
				dir.y = 0;
			}

			else
			{
				dir = directions[i];
			}
			return true; // key is relevant and used
		}

	}
	return false; // uninteresting key
}

void Mario::move()
{
	static int levelUp = 0;
	static int levelDown = 0;

	if ((pBoard->getPaulineX() == x) && (pBoard->getPaulineY() == y)) //Mario got to pauline's location
	{
		isWinning = true;
	}

	if ((pBoard->getHammerX() == x) && (pBoard->getHammerY() == y)) //Mario got to hammers's location
	{
		withHammer = true;
		gotoxy(x, y);
		std::cout << " ";
		pBoard->setCharOnOriginalBoard(x, y, ' ');
		setMarioChar();
	}
	if (isClimbing) //Mario is going up a ladder
	{
		if ((pBoard->getChar(x, y) != 'H') && (isFloor(x, y))) //Mario got to the end of the ladder
		{
			dir.x = dir.y = 0;
			isClimbing = false;
		}
	}

	if (isJumping) //Mario is jumping
	{
		if (levelUp == JUMP) //Mario jumped 2 chars up
		{
			//Stopping the jump
			dir.y = 0;
			isJumping = false;
			levelUp = 0;
			isFalling = true;
		}
		else
		{
			levelUp++; //Mario is going up one char
		}
	}

	if (pBoard->getChar(x, y + 1) == ' ' && !isJumping) //Mario is not on the floor
	{
		//Mario is going down
		levelDown++;
		isFalling = true;
	}

	if (isFalling) //Mario is falling
	{
		dir.y = 1;

		if (isFloor(x, y)) //Mario got to the floor
		{
			if (levelDown >= MAX_HEIGHT) //Mairo dies
				marioDied();

			//Mario stops falling
			isFalling = false;
			levelDown = 0;
			dir.y = 0;
		}
	}

	if (isDownLadder) //Mario is going down in a ladder
	{
		if ((pBoard->getChar(x, y) == 'H') && (isFloor(x, y))) //Mario got to the end of the ladder
		{
			dir.x = dir.y = 0;
			isDownLadder = false;
		}
	}

	int newX = x + dir.x;
	int newY = y + dir.y;
	if (pBoard->getChar(newX, newY) == BORDER) { //Mario got to a border
		dir = { 0, 0 };
	}
	else
	{
		x = newX;
		y = newY;
	}
}

bool Mario::canJump(int x, int y) const //Checks if mario can jump
{
	if ((dir.x == 1) && ((isFloor(x + 2, y - 3)) || (isFloor(x + 1, y - 2)))) //To the right direction there is a ceiling
		return false;
	else if ((dir.x == -1) && ((isFloor(x - 2, y - 3)) || (isFloor(x - 1, y - 2)))) //To the left direction there is a ceiling
		return false;
	else if ((dir.x == 0) && ((isFloor(x - 2, y - 3)) || (isFloor(x - 1, y - 2)))) //Stay and there is a ceiling
		return false;
	return true;
}

void Mario::drawHearts(int x, int y, int choice) const //Draws mario's lives
{
	if (!pBoard->getIsSilent())
	{
		gotoxy(x + 1, y);
		if (choice == NO_COLOR) //Draw without color
		{
			for (int i = 0; i < lives; i++)
			{
				std::cout << "<3 ";
			}
		}
		else //Draw with color
		{
			GameConfig g;
			for (int i = 0; i < lives; i++)
			{
				g.setColor(FOREGROUND_RED);
				std::cout << "\3 ";
			}
			g.setColor(g.BASIC);
		}
	}
}

void Mario::marioDied() //Mario lost a life
{
	lives--;
	isDead = true;

	eraseOnBoard();
	gotoxy(70, 1);
	for (int i = 0; i < (3 - lives); i++)
	{
		std::cout << "   ";
	}
}



