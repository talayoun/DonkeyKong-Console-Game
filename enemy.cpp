#include "enemy.h"

bool Enemy::crashingWithMario() const //If the enemy crashes with mario while moving
{
	if ((pMario->getX() == x) && (pMario->getY() == y))
		return true;
	if ((pMario->getDirX() + dir.x == 0) && (pMario->getDirY() + dir.y == 0) && (abs(pMario->getX() - x) == 1) && (pMario->getY() == y))
		return true;
	return false;
}
void Enemy::explode()
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
}

