#pragma once

#define CONSOLE_WIDTH 64
#define CONSOLE_HEIGHT 32

extern COORD		Csize;
extern SMALL_RECT	Crect;
extern int			nScreenIndex;
extern HANDLE		hBuffer[2];

void Init(HANDLE hBuffer[]);