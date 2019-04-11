#pragma once

class Entity {
public:
	double posX;
	double posY;
	int targetX;
	int targetY;
	bool isOnField;

	Entity(int posX, int posY);
	~Entity();

	void moveUp();
	void moveDown();
	void moveRight();
	void moveLeft();
};