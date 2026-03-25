#pragma once
#include "enemy.h"

constexpr int BORDER1_X = 78;
constexpr int BORDER_Y = 23;
constexpr int BORDER2_X = 2;
constexpr int MARIO_RADIUS = 2;
constexpr int BARRELS_ARR_SIZE = 8;
constexpr int MAX_STEPS = 18;

class Barrel : public Enemy
{ 
	static constexpr char barrelChar = 'O';
	bool isFalling = false;
	bool isExploded = false;
	int countingSteps = 0;
	int levelDown = 0;
	GameConfig g;

public:
	Barrel() : Enemy(0, 0, barrelChar, nullptr) {}
	Barrel(int startX, int startY, char c = barrelChar, Board* b = nullptr) : Enemy(startX, startY, c, b) {}

	int getColorChoice() const override {
		return g.YELLOW;
	}

	void keyByFloor();
	void move() override;
	void explode() override;
	int getCountingSteps() const {
		return countingSteps;
	}
	void resetSteps() {
		countingSteps = 0;
	}
	void resetIsExploded() {
		isExploded = false;
	}
	bool getIsExploded() const {
		return isExploded;
	}
	void exploded() {
		isExploded = true;
	}
	bool barrelOnFloorsEdge(int x, int y) const;
	bool isMarioClose(int marioX, int marioY) const;
	void setEnemy() override;
	void setStartPos(int _x = 0, int _y = 0) override {
		setX(_x + 1);
		setY(_y);
	}
	virtual ~Barrel() {}
};

