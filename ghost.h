#pragma once
#include "enemy.h"


class Ghost : public Enemy
{ 
	static constexpr char ghostChar = 'x';
protected:
	bool isMovingRight = false;
	bool isMovingLeft = false;
	GameConfig g;

public:
	Ghost(int startX, int startY, char c = ghostChar, Board* b = nullptr) : Enemy(startX, startY, c, b) {}

	int getColorChoice() const override {
		return g.GREY;
	}
	bool getIsMovingRight() const
	{
		return isMovingRight;
	}

	void setIsMovingRight(bool val)
	{
		isMovingRight = val;
	}

	void setIsMovingLeft(bool val)
	{
		isMovingLeft = val;
	}

	virtual void keyByFloor();
	virtual void move() override;
	void setEnemy() override;
	virtual std::unique_ptr<Ghost> duplicate() const {
		return std::make_unique<Ghost>(*this);
	}
	virtual ~Ghost() {}
};



