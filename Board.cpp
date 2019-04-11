#include "Board.h"

Board::Board(int bSizeX, int bSizeY) {
	this->bSizeX = bSizeX;
	this->bSizeY = bSizeY;

	board = (Field ***)malloc(bSizeY * sizeof(*board));

	for(int i = 0; i < bSizeY; i++) {
		board[i] = (Field **)malloc(bSizeX * sizeof(**board));
	}

	for(int i = 0; i < bSizeY; i++) {
		for(int j = 0; j < bSizeX; j++) {
			board[i][j] = new Field();
		}
	}
}

Board::~Board() {
	for(int i = 0; i < bSizeY; i++) {
		for(int j = 0; j < bSizeX; j++)
			delete board[i][j];
		free(board[i]);
	}
	free(board);

	for(int i = 0; i < entitiesLength; i++) {
		delete entities[i];
	}

	delete player;
}

void Board::setValue(int x, int y, char value) {
	board[y][x]->value = value;
}

char Board::getValue(int x, int y) {
	return board[y][x]->value;
}

Field *Board::getField(int x, int y) {
	return board[y][x];
}

void Board::loadFromFile(const char *fileName) {
	FILE *f = fopen(fileName, "r");
	char c;
	int counterX = 0;
	int counterY = 0;
	int entityCounter = 0;
	int sizeX = 0;
	int sizeY = 0;

	if(f != NULL) {
		while(fgetc(f) != '\n')
			sizeX++;
		fseek(f, 0L, 0);
		while((c = fgetc(f)) != EOF) {
			if(c == '\n')
				sizeY++;
			else if(c == 'd' || c == 'f')
				entityCounter++;
		}
		sizeY++;
		fseek(f, 0L, 0);

		entitiesLength = entityCounter;
		int *test = (int *)malloc(10 * sizeof(test));
		entities = (Entity **)malloc(entityCounter * sizeof(*entities));
		entityCounter = 0;

		while((c = fgetc(f)) != EOF) {
			if(c == '\n') {
				counterX = 0;
				counterY++;
			} else {
				if(c == 'd' || c == 'e')
					setValue(counterX, counterY, 'b');
				else if(c == 'f' || c == 'g')
					setValue(counterX, counterY, 'c');
				else
					setValue(counterX, counterY, c);

				if(c == 'd' || c == 'f') {
					entities[entityCounter] = new Entity(counterX, counterY);
					getField(counterX, counterY)->entity = entities[entityCounter];
					if(c == 'f')
						getField(counterX, counterY)->entity->isOnField = true;
					entityCounter++;
				} else if(c == 'e' || c == 'g') {
					player = new Player(counterX, counterY);
				}
				counterX++;
			}
		}
		fclose(f);
		setShadows();
	}
}

bool Board::moveUp() {
	if(getValue(player->targetX, player->targetY - 1) != 'a' && player->targetX == player->posX && player->targetY == player->posY) {
		if(getField(player->targetX, player->targetY - 1)->entity != nullptr) {
			if(getValue(player->targetX, player->targetY - 2) != 'a' && getField(player->targetX, player->targetY - 2)->entity == nullptr) {
				getField(player->targetX, player->targetY - 1)->entity->moveUp();
				getField(player->targetX, player->targetY - 2)->entity = getField(player->targetX, player->targetY - 1)->entity;
				getField(player->targetX, player->targetY - 1)->entity = nullptr;
				if(getValue(player->targetX, player->targetY - 2) == 'c')
					getField(player->targetX, player->targetY - 2)->entity->isOnField = true;
				else
					getField(player->targetX, player->targetY - 2)->entity->isOnField = false;
				player->moveUp();
				setShadows();
				return true;
			}
		} else {
			player->moveUp();
			setShadows();
			return true;
		}
	}
	return false;
}

bool Board::moveDown() {
	if(getValue(player->targetX, player->targetY + 1) != 'a' && player->targetX == player->posX && player->targetY == player->posY) {
		if(getField(player->targetX, player->targetY + 1)->entity != nullptr) {
			if(getValue(player->targetX, player->targetY + 2) != 'a' && getField(player->targetX, player->targetY + 2)->entity == nullptr) {
				getField(player->targetX, player->targetY + 1)->entity->moveDown();
				getField(player->targetX, player->targetY + 2)->entity = getField(player->targetX, player->targetY + 1)->entity;
				getField(player->targetX, player->targetY + 1)->entity = nullptr;
				if(getValue(player->targetX, player->targetY + 2) == 'c')
					getField(player->targetX, player->targetY + 2)->entity->isOnField = true;
				else
					getField(player->targetX, player->targetY + 2)->entity->isOnField = false;
				player->moveDown();
				setShadows();
				return true;
			}
		} else {
			player->moveDown();
			setShadows();
			return true;
		}
	}
	return false;
}

bool Board::moveRight() {
	if(getValue(player->targetX + 1, player->targetY) != 'a' && player->targetX == player->posX && player->targetY == player->posY) {
		if(getField(player->targetX + 1, player->targetY)->entity != nullptr) {
			if(getValue(player->targetX + 2, player->targetY) != 'a' && getField(player->targetX + 2, player->targetY)->entity == nullptr) {
				getField(player->targetX + 1, player->targetY)->entity->moveRight();
				getField(player->targetX + 2, player->targetY)->entity = getField(player->targetX + 1, player->targetY)->entity;
				getField(player->targetX + 1, player->targetY)->entity = nullptr;
				if(getValue(player->targetX + 2, player->targetY) == 'c')
					getField(player->targetX + 2, player->targetY)->entity->isOnField = true;
				else
					getField(player->targetX + 2, player->targetY)->entity->isOnField = false;
				player->moveRight();
				setShadows();
				return true;
			}
		} else {
			player->moveRight();
			setShadows();
			return true;
		}
	}
	return false;
}

bool Board::moveLeft() {
	if(getValue(player->targetX - 1, player->targetY) != 'a' && player->targetX == player->posX && player->targetY == player->posY) {
		if(getField(player->targetX - 1, player->targetY)->entity != nullptr) {
			if(getValue(player->targetX - 2, player->targetY) != 'a' && getField(player->targetX - 2, player->targetY)->entity == nullptr) {
				getField(player->targetX - 1, player->targetY)->entity->moveLeft();
				getField(player->targetX - 2, player->targetY)->entity = getField(player->targetX - 1, player->targetY)->entity;
				getField(player->targetX - 1, player->targetY)->entity = nullptr;
				if(getValue(player->targetX - 2, player->targetY) == 'c')
					getField(player->targetX - 2, player->targetY)->entity->isOnField = true;
				else
					getField(player->targetX - 2, player->targetY)->entity->isOnField = false;
				player->moveLeft();
				setShadows();
				return true;
			}
		} else {
			player->moveLeft();
			setShadows();
			return true;
		}
	}
	return false;
}

bool Board::checkEnd() {
	for(int i = 0; i < entitiesLength; i++) {
		if(!entities[i]->isOnField)
			return false;
	}

	return true;
}

bool Board::findPath(int x, int y) {
	for(int i = 0; i < bSizeY; i++) {
		for(int j = 0; j < bSizeX; j++) {
			getField(j, i)->trackLevel = 0;
		}
	}

	setTrackLevel(x, y, 100);

	if(getField(player->targetX, player->targetY - 1)->trackLevel > 0) {
		return true;
	}
	if(getField(player->targetX, player->targetY + 1)->trackLevel > 0) {
		return true;
	}
	if(getField(player->targetX - 1, player->targetY)->trackLevel > 0) {
		return true;
	}
	if(getField(player->targetX + 1, player->targetY)->trackLevel > 0) {
		return true;
	}

	return false;
}

void Board::setTrackLevel(int x, int y, int trackLevel) {
	if(getField(x, y)->entity == nullptr && getValue(x, y) != 'a') {
		getField(x, y)->trackLevel = trackLevel;

		if(x > 0) {
			if(getField(x - 1, y)->trackLevel < trackLevel)
				setTrackLevel(x - 1, y, trackLevel - 1);
		}
		if(x + 1 < bSizeX) {
			if(getField(x + 1, y)->trackLevel < trackLevel)
				setTrackLevel(x + 1, y, trackLevel - 1);
		}
		if(y > 0) {
			if(getField(x, y - 1)->trackLevel < trackLevel)
				setTrackLevel(x, y - 1, trackLevel - 1);
		}
		if(y + 1 < bSizeY) {
			if(getField(x, y + 1)->trackLevel < trackLevel)
				setTrackLevel(x, y + 1, trackLevel - 1);
		}
	}
}

void Board::movePlayer() {
	int maxTrackLevel = 0;
	int whereMove = -1; // 0 - góra, 1 - prawo, 2 - dó³, 3 - lewo;
	
	if(getField(player->targetX,player->targetY - 1)->trackLevel > maxTrackLevel) {
		maxTrackLevel = getField(player->targetX,player->targetY - 1)->trackLevel;
		whereMove = 0;
	}
	if(getField(player->targetX,player->targetY + 1)->trackLevel > maxTrackLevel) {
		maxTrackLevel = getField(player->targetX,player->targetY + 1)->trackLevel;
		whereMove = 2;
	}
	if(getField(player->targetX - 1,player->targetY)->trackLevel > maxTrackLevel) {
		maxTrackLevel = getField(player->targetX - 1,player->targetY)->trackLevel;
		whereMove = 3;
	}
	if(getField(player->targetX + 1,player->targetY)->trackLevel > maxTrackLevel) {
		maxTrackLevel = getField(player->targetX + 1,player->targetY)->trackLevel;
		whereMove = 1;
	}

	if(whereMove == 0)
		player->moveUp();
	else if(whereMove == 1)
		player->moveRight();
	else if(whereMove == 2)
		player->moveDown();
	else if(whereMove == 3)
		player->moveLeft();

	setShadows();
}

void Board::setShadows() {
	for(int i = 0; i < bSizeY; i++) {
		for(int j = 0; j < bSizeX; j++) {
			getField(j, i)->darknessLevel = 0;
		}
	}

	setDarknessLevel(player->targetX, player->targetY, 100, player->targetX, player->targetY);
}

void Board::setDarknessLevel(int x, int y, int darknessLevel, int lastChangedX, int lastChangedY) {
	if(abs(lastChangedX - x) + abs(lastChangedY - y) > 1) {
		if(lastChangedX != x && lastChangedY != y) {
			lastChangedX = x;
			lastChangedY = y;
			darknessLevel -= 2;
		}
	}

	if(getField(x, y)->entity == nullptr && getValue(x, y) != 'a') {
		getField(x, y)->darknessLevel = darknessLevel;

		if(x > 0) {
			if(getField(x - 1, y)->darknessLevel < darknessLevel)
				setDarknessLevel(x - 1, y, darknessLevel - 1, lastChangedX, lastChangedY);
		}
		if(x + 1 < bSizeX) {
			if(getField(x + 1, y)->darknessLevel < darknessLevel)
				setDarknessLevel(x + 1, y, darknessLevel - 1, lastChangedX, lastChangedY);
		}
		if(y > 0) {
			if(getField(x, y - 1)->darknessLevel < darknessLevel)
				setDarknessLevel(x, y - 1, darknessLevel - 1, lastChangedX, lastChangedY);
		}
		if(y + 1 < bSizeY) {
			if(getField(x, y + 1)->darknessLevel < darknessLevel)
				setDarknessLevel(x, y + 1, darknessLevel - 1, lastChangedX, lastChangedY);
		}
	}
}