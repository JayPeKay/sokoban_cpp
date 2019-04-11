#pragma once

#include "Entity.h"

class Field {
public:
	char value;
	int darknessLevel;
	int trackLevel;
	Entity *entity;

	Field();
	~Field();

private:
};