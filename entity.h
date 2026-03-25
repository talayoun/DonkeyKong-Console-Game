#pragma once

#include <iostream>
#include "point.h"


class Entity : public Point
{
public:
	Entity(int startX, int startY, char c, Board* b) : Point(startX, startY, c, b){}

	virtual void move() = 0;
	bool isFloor(int x, int y) const;
	virtual ~Entity(){}
};

