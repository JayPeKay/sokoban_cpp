#include <windows.h>

#include "Window.h"

int main(int argc, char **argv) {
	const HWND hwndDesktop = GetDesktopWindow();
	RECT desktop;
	GetWindowRect(hwndDesktop, &desktop);

	const int marginRight = 100;
	const int marginLeft = 100;
	const int marginTop = 100;
	const int marginBot = 100;

	const int width = desktop.right - marginRight - marginLeft;
	const int height = desktop.bottom - marginTop - marginBot;

	Window window(width, height);
	return 0;
}