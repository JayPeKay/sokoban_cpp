#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "Field.h"
#include "Entity.h"
#include "Player.h"

class Board {
public:
	int x;
	int y;
	int bSizeX;
	int bSizeY;
	Player *player;
	Entity **entities;
	int entitiesLength;

	Board(int bSizeX, int bSizeY);
	~Board();

	void setValue(int x, int y, char value);
	char getValue(int x, int y);
	Field *getField(int x, int y);
	void loadFromFile(const char *fileName);

	bool moveUp();
	bool moveDown();
	bool moveRight();
	bool moveLeft();
	bool checkEnd();
	bool findPath(int x, int y);
	void setTrackLevel(int x, int y, int trackLevel);
	void movePlayer();
	void setDarknessLevel(int x, int y, int darknessLevel, int lastChangedX, int lastChangedY);

private:
	Field ***board;

	void setShadows();
};