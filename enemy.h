#pragma once

#include "entity.h"
#include "mario.h"


class Enemy: public Entity
{
protected:
    Mario* pMario = nullptr;
	bool isMoving = false;

public:
	Enemy(int startX, int startY, char c, Board* b) : Entity(startX, startY, c, b) {}

    void setMario(Mario& mario) {
        pMario = &mario;
    }

    void setIsMoving(bool value) {
        isMoving = value;
    }

    bool getIsMoving() const {
        return isMoving;
    }

    bool crashingWithMario() const;

    virtual void setEnemy() = 0;
    virtual void explode();
    virtual ~Enemy() {}
};

