#pragma once
#include "ghost.h"

class AdvancedGhost : public Ghost
{
	static constexpr char advancedGhostChar = 'X';
	bool isClimbing = false;
public:
	AdvancedGhost(int startX, int startY, char c = advancedGhostChar, Board* b = nullptr) : Ghost(startX, startY, c, b) {}
	void keyByFloor() override;
	void move() override;
	std::unique_ptr<Ghost> duplicate() const override {
		return std::make_unique<AdvancedGhost>(*this);
	}
};

