#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

#include "Init.h"

COORD		Csize;
SMALL_RECT	Crect;
int			nScreenIndex;
HANDLE		hBuffer[2];

void Init(HANDLE hBuffer[])
{
	nScreenIndex = 0;

	srand(time(NULL));

	Csize.X = CONSOLE_WIDTH * 2;
	Csize.Y = CONSOLE_HEIGHT;

	Crect.Left = 0;
	Crect.Right = Csize.X - 1;
	Crect.Top = 0;
	Crect.Bottom = Csize.Y - 1;

	hBuffer[0] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	SetConsoleScreenBufferSize(hBuffer[0], Csize);
	SetConsoleWindowInfo(hBuffer[0], TRUE, &Crect);

	hBuffer[1] = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);

	SetConsoleScreenBufferSize(hBuffer[1], Csize);
	SetConsoleWindowInfo(hBuffer[1], TRUE, &Crect);

	SetWindowLong(
		GetConsoleWindow(),
		GWL_STYLE,
		GetWindowLong(
			GetConsoleWindow(),
			GWL_STYLE
		) &
		~WS_MAXIMIZEBOX &
		~WS_SIZEBOX
	);

	char s[30];
	sprintf(s, "mode con lines=%d cols=%d", CONSOLE_HEIGHT, CONSOLE_WIDTH * 2);
	system(s);

	SetConsoleTitleA("Console Dungeon");

	CONSOLE_CURSOR_INFO cInfo = { 1,0 };
	SetConsoleCursorInfo(hBuffer[0], &cInfo);
	SetConsoleCursorInfo(hBuffer[1], &cInfo);
	SetConsoleMode(
		GetStdHandle(STD_INPUT_HANDLE),
		ENABLE_EXTENDED_FLAGS | (ENABLE_MOUSE_INPUT & ~ENABLE_QUICK_EDIT_MODE)
	);

	SetConsoleTextAttribute(hBuffer[0], 0x0f);
	SetConsoleTextAttribute(hBuffer[1], 0x0f);
}