#include "Entity.h"

Entity::Entity(int posX, int posY) {
	this->posX = posX;
	this->posY = posY;
	this->targetX = posX;
	this->targetY = posY;
	this->isOnField = false;
}

Entity::~Entity() {}

void Entity::moveUp() {
	targetY--;
}

void Entity::moveDown() {
	targetY++;
}

void Entity::moveRight() {
	targetX++;
}

void Entity::moveLeft() {
	targetX--;
}