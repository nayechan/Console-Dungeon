#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "Init.h"

WORD attributeBuffer[CONSOLE_HEIGHT][CONSOLE_WIDTH * 2 + 1];
char textBuffer[CONSOLE_HEIGHT][CONSOLE_WIDTH * 2 + 1];

void gotoXY(int x, int y)
{
	COORD cur = { x,y };
	SetConsoleCursorPosition(hBuffer[nScreenIndex], cur);
}

void printXY(int x, int y, int bg, int fg, char * ch, int size)
{
	WORD attribute = bg << 4 | fg;

	for (int i = 0; i < size; ++i)
	{
		if (x + i >= CONSOLE_WIDTH * 2) break;
		textBuffer[y][x + i] = ch[i];
		attributeBuffer[y][x + i] = attribute;
	}
}

void BufferFlip()
{
	DWORD written;
	for (int i = 0; i < CONSOLE_HEIGHT; ++i)
	{
		COORD cur = { 0,i };
		WriteConsoleOutputAttribute(hBuffer[nScreenIndex], attributeBuffer[i], CONSOLE_WIDTH * 2, cur, &written);
		WriteConsoleOutputCharacterA(hBuffer[nScreenIndex], textBuffer[i], CONSOLE_WIDTH * 2, cur, &written);
	}
	SetConsoleActiveScreenBuffer(hBuffer[nScreenIndex]);
	nScreenIndex = !nScreenIndex; 
	for (int i = 0; i < CONSOLE_HEIGHT; ++i)
	{
		for (int j = 0; j < CONSOLE_WIDTH * 2; ++j)
		{
			textBuffer[i][j] = 0;
			attributeBuffer[i][j] = 0;
		}
	}
}

void BufferClear()
{
	COORD Coor = { 0, 0 }; //세번째 줄의 함수를 위한 구조체
	DWORD dw; //세번째 줄의 함수를 위한 구조체
	FillConsoleOutputCharacterW(hBuffer[nScreenIndex], ' ', Csize.X * Csize.Y, Coor, &dw);
	FillConsoleOutputAttribute(hBuffer[nScreenIndex], 0x00, Csize.X * Csize.Y, Coor, &dw);
}

int _GetKeyState(int key)
{
	static int keyState[256];
	if (keyState[key] == 0)
	{
		if (GetAsyncKeyState(key) & 0x8000)
		{
			keyState[key] = 1;
			return 1;
		}
		else return 0;
	}
	else
	{
		if (GetAsyncKeyState(key) & 0x8000)
			return 2;

		else
		{
			keyState[key] = 0;
			return -1;
		}
	}
}

