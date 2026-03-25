#pragma once

#include <iostream>
#include "general.h"
#include "entity.h"

constexpr int LIVES = 3;
constexpr int NO_LIVES = 0;
constexpr int JUMP = 2;
constexpr int MAX_HEIGHT = 5;

//class Board;

class Mario : public Entity
{ 
	char marioChar = '@';
	bool isClimbing = false;
	bool isJumping = false;
	bool isFalling = false;
	bool isDownLadder = false;
	bool isDead = false;
	bool isWinning = false;
	bool withHammer = false;
	int lives = LIVES;
	GameConfig g;
	static constexpr char keys[] = { 'w', 'a', 'x', 'd', 's' ,'p'};
	static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);


public:
	Mario(int startX, int startY, char c, Board* b = nullptr) : Entity(startX, startY, c, b) { marioChar = c; }

	int getColorChoice() const override {
		return g.RED;
	}
	char getChar() const override { //Return the object's char
		return marioChar;
	}

	bool keyPressed(char key);
	void move() override;
	bool canJump(int x, int y) const;
	void drawHearts(int x, int y, int choice = 0)const;
	void marioDied();
	int getLives() const { //Returns current lives
		return lives;
	}
	void setLives() { //Resets to starting lives count
		lives = LIVES;
	}
	bool getIsDead() const { //Returns if mario is dead
		return isDead;
	}
	void setIsWinning() { //Resets winning indicator to defult
		isWinning = false;
	}
	bool getIsWinning() const { //Retunts if mario won
		return isWinning;
	}
	bool getWithHammer() const { //Retunts if mario has a hammer
		return withHammer;
	}
	
	void setIsDead() { //Resets mario's death to defult
		isDead = false;;
	}
	void resetWithHammer() { //Resets mario's hammer status
		withHammer = false;;
	}
	
	char getMarioChar() const {
		return marioChar;
	}
	
	void setMarioChar() //Sets mario's char to indicate that he is with a hammer
	{
		marioChar = '%';
	}
	void resetMarioChar() //Resets mario's char to defult
	{
		marioChar = '@';
	}
	virtual ~Mario(){}
};

