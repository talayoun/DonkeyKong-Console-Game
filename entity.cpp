#include "entity.h"

bool Entity::isFloor(int x, int y) const //Checks if the entity is on the floor
{
	if ((pBoard->getChar(x, y + 1) == '<') || (pBoard->getChar(x, y + 1) == '>') || (pBoard->getChar(x, y + 1) == '=') || (pBoard->getChar(x, y + 1) == 'Q') || (pBoard->getChar(x, y + 1) == '^'))
		return true;
	return false;
}
