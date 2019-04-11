#pragma once
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SDL-2.0.7\include\SDL_ttf.h"

#include "Board.h"
#include "String.h"
#include "ViewReturn.h"

class Window {
public:
	struct Level {
		int id;
		char title[20];
		char fileName[20];
	};

	struct Record {
		double time;
		int movesNumber;
	};

	Window(int width, int height);
	~Window();

private:
	int width;
	int height;
	int windowBackground;
	int mouseX;
	int mouseY;
	double worldTime;
	int currentLevel;
	int levelsNumber;

	Board *board;
	Level *levels;
	String levelsFolder;

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *screen;
	SDL_Texture *texture;
	SDL_Event event;
	SDL_Surface *ground, *wall, *chest, *field, *ball, *shadow, *light, *congratulation;
	SDL_Surface *button, *buttonHover;
	TTF_Font *fontText, *fontButton;

	bool initSDL();

	void openMenu();
	int startGameLoop();
	int openEndScreen();
	int openLevelsMenu();
	int openLevelAdd();
	int openRank(int id);

	SDL_Rect drawString(int x, int y, const char *text, SDL_Color color = {255, 255, 255});
	bool drawButton(int centerX, int y, const char *text);
	void createBoardFromFile(const char *fileName);
	void animObjects(double move);

	void getLevels();
	bool addLevel(char *title, char *filePath);
	Record *getRecords(int id, int *recordsLength);
	bool addRecord();
	//char *getLevelPath();
};