#pragma once
#include "Entity.h"
class Player :
	public Entity {
public:
	int movesNumber;

	Player(int posX, int posY);
	~Player();

	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
};

