#pragma once

extern WORD attributeBuffer[CONSOLE_HEIGHT][CONSOLE_WIDTH * 2 + 1];
extern char textBuffer[CONSOLE_HEIGHT][CONSOLE_WIDTH * 2 + 1];

void gotoXY(int x, int y);
void printXY(int x, int y, int bg, int fg, char * ch, int len);
void BufferFlip();
void BufferClear();
int _GetKeyState(int key);