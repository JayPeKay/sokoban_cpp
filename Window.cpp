#include "Window.h"

Window::Window(int width, int height):
	levelsFolder("levels/"){
	this->width = width;
	this->height = height;
	this->mouseX = 0;
	this->mouseY = 0;
	this->worldTime = 0;
	this->currentLevel = 0;
	this->levelsNumber = 0;
	getLevels();

	if(initSDL()) {
		openMenu();
	}
}

Window::~Window() {
	if(worldTime > 0)
		delete board;
	free(levels);
}

bool Window::initSDL() {
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	window = SDL_CreateWindow("Sokoban", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
	if(window == nullptr) {
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(window, -false, SDL_RENDERER_ACCELERATED);
	if(renderer == nullptr) {
		printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	screen = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if(screen == NULL) {
		printf("SDL_CreateRGBSurface error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if(texture == NULL) {
		printf("SDL_CreateTexture error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	ground = SDL_LoadBMP("./img/floor.bmp");
	if(ground == NULL) {
		printf("SDL_LoadBMP(floor.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	wall = SDL_LoadBMP("./img/wall.bmp");
	if(wall == NULL) {
		printf("SDL_LoadBMP(wall.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	chest = SDL_LoadBMP("./img/chest.bmp");
	if(chest == NULL) {
		printf("SDL_LoadBMP(chest.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	field = SDL_LoadBMP("./img/field.bmp");
	if(field == NULL) {
		printf("SDL_LoadBMP(field.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	ball = SDL_LoadBMP("./img/ball.bmp");
	if(ball == NULL) {
		printf("SDL_LoadBMP(ball.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	shadow = SDL_LoadBMP("./img/shadow.bmp");
	if(shadow == NULL) {
		printf("SDL_LoadBMP(shadow.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	light = SDL_LoadBMP("./img/light.bmp");
	if(light == NULL) {
		printf("SDL_LoadBMP(light.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	congratulation = SDL_LoadBMP("./img/congratulation.bmp");
	if(shadow == NULL) {
		printf("SDL_LoadBMP(congratulation.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	button = SDL_LoadBMP("./img/button.bmp");
	if(shadow == NULL) {
		printf("SDL_LoadBMP(button.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	buttonHover = SDL_LoadBMP("./img/button_hover.bmp");
	if(shadow == NULL) {
		printf("SDL_LoadBMP(button_hover.bmp) error: %s\n", SDL_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	windowBackground = SDL_MapRGBA(screen->format, 0, 0, 0, 255);

	if(TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return false;
	}

	fontText = TTF_OpenFont("Slabo.ttf", 20);
	if(fontText == NULL) {
		printf("TTF_OpenFont(Slabo.ttf) error: %s\n", TTF_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_CloseFont(fontText);
		TTF_Quit();
		SDL_Quit();
		return false;
	}

	fontButton = TTF_OpenFont("JotiOne.ttf", 25);
	if(fontButton == NULL) {
		printf("TTF_OpenFont(JotiOne.ttf) error: %s\n", TTF_GetError());
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(screen);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_CloseFont(fontText);
		TTF_CloseFont(fontButton);
		TTF_Quit();
		SDL_Quit();
		return false;
	}

	return true;
}

int Window::startGameLoop() {
	char text[128];
	int t1 = SDL_GetTicks();
	int t2;
	int focusedX = -1;
	int focusedY = -1;
	bool isFocused = false;
	bool movePlayer = false;
	double animationDuration = 100;
	double move;
	double delta;
	SDL_Rect dest;
	dest.w = wall->w;
	dest.h = wall->h;

	while(true) {
		SDL_FillRect(screen, NULL, windowBackground);

		isFocused = false;

		t2 = SDL_GetTicks();
		delta = (t2 - t1);
		t1 = t2;
		worldTime += delta * 0.001;

		move = delta / animationDuration;
		animObjects(move);

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					return APP_PAUSE;
				else if(event.key.keysym.sym == SDLK_UP) {
					board->moveUp();
				} else if(event.key.keysym.sym == SDLK_DOWN) {
					board->moveDown();
				}
				else if(event.key.keysym.sym == SDLK_RIGHT) {
					board->moveRight();
				}
				else if(event.key.keysym.sym == SDLK_LEFT) {
					board->moveLeft();
				} else if(event.key.keysym.sym == SDLK_n) {
					delete board;
					createBoardFromFile(levelsFolder + levels[currentLevel].fileName);
					worldTime = 0;
					board->player->movesNumber = 0;
				}
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				if(focusedX >= 0 && focusedY >= 0) {
					if(board->getField(focusedX, focusedY)->entity == nullptr) {
						if(board->findPath(focusedX, focusedY))
							movePlayer = true;
					} else if(focusedX == board->player->posX + 1 && focusedY == board->player->posY) {
						board->moveRight();
					} else if(focusedX == board->player->posX - 1 && focusedY == board->player->posY) {
						board->moveLeft();
					} else if(focusedY == board->player->posY + 1 && focusedX == board->player->posX) {
						board->moveDown();
					} else if(focusedY == board->player->posY - 1 && focusedX == board->player->posX) {
						board->moveUp();
					}
				}
				break;
			case SDL_QUIT:
				return APP_EXIT;
				break;
			}
		}

		if(board->player->targetX == board->player->posX && board->player->targetY == board->player->posY) {
			if(movePlayer && board->getField(board->player->targetX, board->player->targetY)->trackLevel != 100) {
				board->movePlayer();
			} else if(board->getField(board->player->targetX, board->player->targetY)->trackLevel == 100)
				movePlayer = false;
		}

		sprintf(text, "Sokoban, czas trwania = %.1f s", worldTime);
		drawString(screen->w / 2, 10, text);

		for(int i = 0; i < board->bSizeY; i++) {
			for(int j = 0; j < board->bSizeX; j++) {
				dest.x = j * 25 + board->x;
				dest.y = i * 25 + board->y;
				switch(board->getValue(j, i)) {
				case 'a': // wall
					SDL_BlitSurface(wall, NULL, screen, &dest);
					break;
				case 'b': // ground
					SDL_BlitSurface(ground, NULL, screen, &dest);
					break;
				case 'c': // field
					SDL_BlitSurface(field, NULL, screen, &dest);
					break;
				}

				if(board->getField(j, i)->darknessLevel > 95) {
					for(int k = 100 - board->getField(j, i)->darknessLevel; k > 0; k--) {
						SDL_BlitSurface(shadow, NULL, screen, &dest);
					}
				} else if(board->getValue(j, i) == 'a') {
					SDL_BlitSurface(shadow, NULL, screen, &dest);
					SDL_BlitSurface(shadow, NULL, screen, &dest);
					SDL_BlitSurface(shadow, NULL, screen, &dest);
				} else {
					for(int k = 0; k < 10; k++)
						SDL_BlitSurface(shadow, NULL, screen, &dest);
				}

				if(board->getValue(j, i) == 'b' || board->getValue(j, i) == 'c') {
					if(mouseX >= dest.x && mouseX < dest.x + dest.w && mouseY >= dest.y && mouseY < dest.y + dest.h) {
						SDL_BlitSurface(light, NULL, screen, &dest);
						focusedX = j;
						focusedY = i;
						isFocused = true;
					}
				}
			}
		}

		if(!isFocused) {
			focusedX = -1;
			focusedY = -1;
		}

		for(int i = 0; i < board->entitiesLength; i++) {
			dest.x = board->entities[i]->posX * 25 + board->x;
			dest.y = board->entities[i]->posY * 25 + board->y;
			SDL_BlitSurface(chest, NULL, screen, &dest);

			SDL_BlitSurface(shadow, NULL, screen, &dest);
			SDL_BlitSurface(shadow, NULL, screen, &dest);
			SDL_BlitSurface(shadow, NULL, screen, &dest);

			if(mouseX >= dest.x && mouseX < dest.x + dest.w && mouseY >= dest.y && mouseY < dest.y + dest.h)
				SDL_BlitSurface(light, NULL, screen, &dest);
		}

		dest.x = board->player->posX * 25 + board->x;
		dest.y = board->player->posY * 25 + board->y;
		SDL_BlitSurface(ball, NULL, screen, &dest);

		if(board->checkEnd()) {
			return openEndScreen();
		}

		SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

int Window::openEndScreen() {
	int currentButtonHover = -1;
	char text[40];
	char text2[40];
	SDL_Rect time;
	SDL_Rect moves;
	SDL_Rect destEnd;
	destEnd.w = congratulation->w;
	destEnd.h = congratulation->h;
	destEnd.x = screen->w / 2 - congratulation->w / 2;
	destEnd.y = 50;

	sprintf(text, "Twoj czas to: %.3f s", worldTime);
	sprintf(text2, "Ilosc ruchow: %d", board->player->movesNumber);
	addRecord();

	while(true) {
		currentButtonHover = -1;

		SDL_FillRect(screen, NULL, windowBackground);

		SDL_BlitSurface(congratulation, NULL, screen, &destEnd);

		time = drawString(screen->w / 2, destEnd.y + destEnd.h + 20, text);
		moves = drawString(screen->w / 2, time.y + time.h + 10, text2);

		if(currentLevel + 1 < levelsNumber) {
			if(drawButton(screen->w / 2, moves.y + moves.h + 10, "Dalej"))
				currentButtonHover = 0;
			if(drawButton(screen->w / 2, moves.y + moves.h + button->h + 20, "Menu"))
				currentButtonHover = 1;
		} else {
			if(drawButton(screen->w / 2, moves.y + moves.h + 10, "Menu"))
				currentButtonHover = 1;
		}


		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					return APP_BACK;
				else if(event.key.keysym.sym == SDLK_RETURN && currentLevel + 1 < levelsNumber)
					return APP_NEXT_LEVEL;
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				if(currentButtonHover == 0)
					return APP_NEXT_LEVEL;
				else if(currentButtonHover == 1)
					return APP_BACK;
				break;
			case SDL_QUIT:
				return APP_EXIT;
				break;
			}
		}

		SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

void Window::openMenu() {
	int menuLength = 5;
	char *menu[] = {"Nowy bieg", "Poziomy", "Dodaj poziom", "Ranking", "Wyjdz"};
	int currentButtonHover = -1;
	int returnVal = APP_EXIT - 1;
	int returnValRank = APP_EXIT - 1;

	while(true) {
		SDL_FillRect(screen, NULL, windowBackground);

		currentButtonHover = -1;
		for(int i = 0; i < menuLength; i++) {
			if(drawButton(screen->w / 2, (i + 1) * (button->h + 20), menu[i])) {
				currentButtonHover = i;
			}

		}

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					if(returnVal == APP_PAUSE) {
						returnVal = startGameLoop();
					} else {
						return;
					}
				}
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				switch(currentButtonHover) {
				case 0:
					if(worldTime > 0)
						delete board;
					worldTime = 0;
					currentLevel = 0;

					createBoardFromFile(levelsFolder + levels[currentLevel].fileName);
					returnVal = startGameLoop();
					break;
				case 1:
					returnVal = openLevelsMenu();
					if(returnVal >= 0) {
						if(worldTime > 0)
							delete board;
						worldTime = 0;
						currentLevel = returnVal;

						createBoardFromFile(levelsFolder + levels[currentLevel].fileName);
						returnVal = startGameLoop();
					}
					break;
				case 2:
					returnVal = openLevelAdd();
					break;
				case 3:
					do {
						returnVal = openLevelsMenu();

						if(returnVal >= 0) {
							returnValRank = openRank(returnVal);
						} else {
							break;
						}
					} while(returnValRank == APP_BACK);

					if(returnValRank >= APP_EXIT)
						returnVal = returnValRank;
					break;
				case 4:
					return;
					break;
				}
				break;
			case SDL_QUIT:
				return;
				break;
			}
		}

		if(returnVal == APP_EXIT)
			return;
		else if(returnVal == APP_NEXT_LEVEL) {
			if(worldTime > 0)
				delete board;
			worldTime = 0;

			createBoardFromFile(levelsFolder + levels[++currentLevel].fileName);
			returnVal = startGameLoop();
		}

		SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

int Window::openLevelsMenu() {
	int currentButtonHover = -1;
	int returnVal = -1;
	int startDraw = 0;

	while(true) {
		SDL_FillRect(screen, NULL, windowBackground);

		currentButtonHover = -1;

		for(int i = 0; i < levelsNumber; i++) {
			if(drawButton(screen->w / 2, (i + 1) * (button->h + 20) + startDraw, levels[i].title))
				currentButtonHover = i;
		}

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE) {
					return APP_BACK;
				}
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				if(currentButtonHover >= 0)
					return currentButtonHover;
				break;
			case SDL_MOUSEWHEEL:
				if(event.wheel.y == 1 && startDraw < 0) {
					startDraw += 30;
				} else if(event.wheel.y == -1 && levelsNumber * (button->h + 20) + startDraw > height - button->h - 20) {
					startDraw -= 30;
				}
				break;
			case SDL_QUIT:
				return APP_EXIT;
				break;
			}
		}

		SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

int Window::openLevelAdd() {
	bool buttonHovered;
	String enterLevel;
	String title;
	String filePath;
	String filePathMessage = "Sciezka do pliku w folderze ";
	filePathMessage += levelsFolder;
	filePathMessage += "...";
	SDL_Rect titleRect;
	SDL_Rect filePathRect;
	int inputFocused = -1;
	int inputY = 100;
	int t1 = SDL_GetTicks();
	int t2;
	int infoTime = SDL_GetTicks();

	while(true) {
		t2 = SDL_GetTicks();

		SDL_FillRect(screen, NULL, windowBackground);

		if(t2 - infoTime < 2000) {
			drawString(screen->w / 2, 50, enterLevel);
		}

		if(title.length == 0 && inputFocused != 0)
			titleRect = drawString(screen->w / 2, inputY, "Nazwa poziomu...");
		else
			titleRect = drawString(screen->w / 2, inputY, title);

		if(filePath.length == 0 && inputFocused != 1)
			filePathRect = drawString(screen->w / 2, titleRect.y + titleRect.h + 10, filePathMessage);
		else
			filePathRect = drawString(screen->w / 2, titleRect.y + titleRect.h + 10, filePath);

		if(t2 - t1 < 500) {
			if(inputFocused == 0)
				drawString(titleRect.x + titleRect.w + 2, titleRect.y, "|");
			else if(inputFocused == 1)
				drawString(filePathRect.x + filePathRect.w + 2, filePathRect.y, "|");
		} else if(t2 - t1 > 1000)
			t1 = SDL_GetTicks();

		buttonHovered = drawButton(screen->w / 2, filePathRect.y + filePathRect.h + 10, "Dodaj");

		SDL_StartTextInput();
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_TEXTINPUT:
				if(inputFocused == 0)
					title += event.text.text;
				else if(inputFocused == 1)
					filePath += event.text.text;
				break;
			case SDL_MOUSEBUTTONUP:
				if(event.button.button == SDL_BUTTON_RIGHT) {
					if(inputFocused == 0)
						title = SDL_GetClipboardText();
					else if(inputFocused == 1)
						filePath = SDL_GetClipboardText();
				}
				if(mouseX >= titleRect.x && mouseX < titleRect.x + titleRect.w && mouseY >= titleRect.y && mouseY < titleRect.y + titleRect.h)
					inputFocused = 0;
				else if(mouseX >= filePathRect.x && mouseX < filePathRect.x + filePathRect.w && mouseY >= filePathRect.y && mouseY < filePathRect.y + filePathRect.h)
					inputFocused = 1;
				else
					inputFocused = -1;
				if(buttonHovered) {
					if(title.length > 0 && filePath.length > 0) {
						if(addLevel(title, filePath)) {
							free(levels);
							getLevels();
							enterLevel = "Dodano nowy poziom.";
						} else
							enterLevel = "Nie udalo sie dodac nowego poziomu.";
					} else
						enterLevel = "Pola sa wymagane.";
					infoTime = SDL_GetTicks();
				}
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_BACKSPACE) {
					if(inputFocused == 0)
						--title;
					else if(inputFocused == 1)
						--filePath;
				} else if(event.key.keysym.sym == SDLK_ESCAPE) {
					return APP_BACK;
				} else if(event.key.keysym.sym == SDLK_RETURN) {
					if(title.length > 0 && filePath.length > 0) {
						if(addLevel(title, filePath)) {
							free(levels);
							getLevels();
							enterLevel = "Dodano nowy poziom.";
						} else
							enterLevel = "Nie udalo sie dodac nowego poziomu.";
					} else
						enterLevel = "Pola sa wymagane.";
					infoTime = SDL_GetTicks();
				} else if(event.key.keysym.sym == SDLK_TAB) {
					if(inputFocused == 0)
						inputFocused = 1;
					else if(inputFocused == 1)
						inputFocused = 0;
					else if(inputFocused == -1)
						inputFocused = 0;
				}
				break;
			case SDL_QUIT:
				return APP_EXIT;
				break;
			}
		}

		SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

int Window::openRank(int id) {
	int currentButtonHover = -1;
	int sortBy = -1; // 0 - time, 1 - moves number
	bool wasSorted = false;
	int recordsLength = 0;
	Record *records = getRecords(id, &recordsLength);
	String time;
	String movesNumber;
	int posY = 50;
	SDL_Rect lastRecordRect;

	while(true) {
		SDL_FillRect(screen, NULL, windowBackground);

		currentButtonHover = -1;

		if(!wasSorted) {
			if(sortBy == 0) {
				for(int i = 1; i < recordsLength; i++) {
					for(int j = 1; j < recordsLength; j++) {
						if(records[j - 1].time > records[j].time) {
							double temp = records[j - 1].time;
							int temp2 = records[j - 1].movesNumber;

							records[j - 1].time = records[j].time;
							records[j - 1].movesNumber = records[j].movesNumber;

							records[j].time = temp;
							records[j].movesNumber = temp2;
						}
					}
				}
			} else if(sortBy == 1) {
				for(int i = 1; i < recordsLength; i++) {
					for(int j = 1; j < recordsLength; j++) {
						if(records[j - 1].movesNumber > records[j].movesNumber) {
							double temp = records[j - 1].time;
							int temp2 = records[j - 1].movesNumber;

							records[j - 1].time = records[j].time;
							records[j - 1].movesNumber = records[j].movesNumber;

							records[j].time = temp;
							records[j].movesNumber = temp2;
						}
					}
				}
			}

			wasSorted = true;
		}

		for(int i = 0; i < recordsLength; i++) {
			time = records[i].time;
			movesNumber = records[i].movesNumber;
			if(i == 0) {
				drawString(screen->w / 2 - button->w / 2 - 5, button->h + posY + 20, time);
				lastRecordRect = drawString(screen->w / 2 + button->w / 2 + 5, button->h + posY + 20, movesNumber);
			} else {
				drawString(screen->w / 2 - button->w / 2 - 5, lastRecordRect.y + lastRecordRect.h + 10, time);
				lastRecordRect = drawString(screen->w / 2 + button->w / 2 + 5, lastRecordRect.y + lastRecordRect.h + 10, movesNumber);
			}
		}

		if(drawButton(screen->w / 2 - button->w / 2 - 5, posY, "Czas"))
			currentButtonHover = 0;
		if(drawButton(screen->w / 2 + button->w / 2 + 5, posY, "Ruchy"))
			currentButtonHover = 1;

		SDL_StartTextInput();
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_MOUSEBUTTONUP:
				switch(currentButtonHover) {
				case 0:
					wasSorted = false;
					sortBy = 0;
					break;
				case 1:
					wasSorted = false;
					sortBy = 1;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				mouseX = event.motion.x;
				mouseY = event.motion.y;
				break;
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					return APP_BACK;
				break;
			case SDL_QUIT:
				return APP_EXIT;
				break;
			}
		}

		SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

void Window::getLevels() {
	FILE *f = fopen("levels.conf", "r");
	char c;

	if(f != NULL) {
		levelsNumber = 0;
		while((c = fgetc(f)) != EOF)
			if(c == '\n')
				levelsNumber++;
		rewind(f);

		levels = (Level *)malloc(levelsNumber * sizeof(*levels));

		for(int i = 0; i < levelsNumber; i++) {
			fscanf(f, " %d ", &levels[i].id);
			fscanf(f, " \"%[^\"]\" = ", levels[i].title);
			fscanf(f, " \"%[^\"]\" ", levels[i].fileName);
		}

		fclose(f);
	}
}

bool Window::addLevel(char *title, char *filePath) {
	FILE *f = fopen("levels.conf", "a");
	FILE *f2 = fopen(filePath, "r");

	if(f != NULL && f2 != NULL) {
		fprintf(f, "%d \"%s\" = \"%s\"\n", levels[levelsNumber - 1].id + 1, title, filePath);
		fclose(f);
		return true;
	}

	return false;
}

Window::Record *Window::getRecords(int id, int *recordsLength) {
	FILE *f = fopen("records.txt", "r");
	Record *records;
	int recordsNumber = 0;
	int counter = 0;
	char c;
	int checkedId;

	if(f != NULL) {
		while(fscanf(f, " %d ", &checkedId) != EOF) {
			if(checkedId == id)
				recordsNumber++;
			while((c = fgetc(f)) != EOF)
				if(c == '\n')
					break;
		}
		rewind(f);

		records = (Record *)malloc(recordsNumber * sizeof(*records));

		while(counter < recordsNumber && fscanf(f, "%d", &checkedId) != EOF) {
			fscanf(f, "%lf %d", &records[counter].time, &records[counter].movesNumber);
			if(checkedId == id) {
				counter++;
			}
		}

		fclose(f);
		*recordsLength = recordsNumber;
		return records;
	}

	return nullptr;
}

bool Window::addRecord() {
	FILE *f = fopen("records.txt", "a");

	if(f != NULL) {
		fprintf(f, "%d %.3f %d\n", currentLevel, worldTime, board->player->movesNumber);
		fclose(f);
		return true;
	}

	return false;
}

bool Window::drawButton(int centerX, int y, const char *text) {
	SDL_Rect destButton;
	SDL_Rect destFont;
	bool mouseOver;

	destButton.w = button->w;
	destButton.h = button->h;
	destButton.x = centerX - button->w / 2;
	destButton.y = y;
	mouseOver = (mouseX >= destButton.x && mouseX < destButton.x + destButton.w && mouseY >= destButton.y && mouseY < destButton.y + destButton.h);
	if(mouseOver)
		SDL_BlitSurface(buttonHover, NULL, screen, &destButton);
	else
		SDL_BlitSurface(button, NULL, screen, &destButton);

	SDL_Color color = {255, 255, 255};
	SDL_Surface *surface = TTF_RenderText_Solid(fontButton, text, color);
	destFont.w = surface->w;
	destFont.h = surface->h;
	destFont.x = centerX - surface->w / 2;
	destFont.y = y + button->h / 2 - surface->h / 2;
 	SDL_BlitSurface(surface, NULL, screen, &destFont);

	SDL_FreeSurface(surface);
	return mouseOver;
}

SDL_Rect Window::drawString(int x, int y, const char *text, SDL_Color color) {
	SDL_Rect dest;
	SDL_Surface *surface;
	if(strlen(text) > 0)
		surface = TTF_RenderText_Solid(fontText, text, color);
	else
		surface = TTF_RenderText_Solid(fontText, " ", color);
	dest.h = surface->h;
	dest.w = surface->w;
	dest.x = x - surface->w / 2;
	dest.y = y;
	SDL_BlitSurface(surface, NULL, screen, &dest);
	SDL_FreeSurface(surface);
	return dest;
}

void Window::createBoardFromFile(char *fileName) {
	FILE *f = fopen(fileName, "r");
	char c;
	int sizeX = 0;
	int sizeY = 0;

	if(f != NULL) {
		while(fgetc(f) != '\n')
			sizeX++;
		fseek(f, 0L, 0);
		while((c = fgetc(f)) != EOF)
			if(c == '\n')
				sizeY++;
		sizeY++;

		fclose(f);

		board = new Board(sizeX, sizeY);
		board->loadFromFile(fileName);
		board->x = width / 2 - board->bSizeX * 25 / 2;
		board->y = height / 2 - board->bSizeY * 25 / 2;
	}
}

void Window::animObjects(double move) {
	for(int i = 0; i < board->entitiesLength; i++) {
		if(board->entities[i]->posX != board->entities[i]->targetX) {
			if(board->entities[i]->targetX > board->entities[i]->posX) {
				if(board->entities[i]->posX + move > board->entities[i]->targetX)
					board->entities[i]->posX = board->entities[i]->targetX;
				else
					board->entities[i]->posX += move;
			} else {
				if(board->entities[i]->posX - move < board->entities[i]->targetX)
					board->entities[i]->posX = board->entities[i]->targetX;
				else
					board->entities[i]->posX -= move;
			}
		} else if(board->entities[i]->posY != board->entities[i]->targetY) {
			if(board->entities[i]->targetY > board->entities[i]->posY) {
				if(board->entities[i]->posY + move > board->entities[i]->targetY)
					board->entities[i]->posY = board->entities[i]->targetY;
				else
					board->entities[i]->posY += move;
			} else {
				if(board->entities[i]->posY - move < board->entities[i]->targetY)
					board->entities[i]->posY = board->entities[i]->targetY;
				else
					board->entities[i]->posY -= move;
			}
		}
	}

	if(board->player->posX != board->player->targetX) {
		if(board->player->targetX > board->player->posX) {
			if(board->player->posX + move > board->player->targetX)
				board->player->posX = board->player->targetX;
			else
				board->player->posX += move;
		} else {
			if(board->player->posX - move < board->player->targetX)
				board->player->posX = board->player->targetX;
			else
				board->player->posX -= move;
		}
	} else if(board->player->posY != board->player->targetY) {
		if(board->player->targetY > board->player->posY) {
			if(board->player->posY + move > board->player->targetY)
				board->player->posY = board->player->targetY;
			else
				board->player->posY += move;
		} else {
			if(board->player->posY - move < board->player->targetY)
				board->player->posY = board->player->targetY;
			else
				board->player->posY -= move;
		}
	}
}