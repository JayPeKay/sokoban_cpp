#include "Player.h"

Player::Player(int posX, int posY) :
	Entity(posX, posY) {
	this->movesNumber = 0;
}

Player::~Player() {}

void Player::moveUp() {
	Entity::moveUp();
	movesNumber++;
}

void Player::moveDown() {
	Entity::moveDown();
	movesNumber++;
}

void Player::moveRight() {
	Entity::moveRight();
	movesNumber++;
}

void Player::moveLeft() {
	Entity::moveLeft();
	movesNumber++;
}