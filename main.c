#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>

#include "Init.h"
#include "MainScene.h"
#include "InGame.h"

int main()
{
	Init(hBuffer);

	MainScene(hBuffer);
	InGame(hBuffer);

	CloseHandle(hBuffer[0]);
	CloseHandle(hBuffer[1]);
	
	return 0;
}