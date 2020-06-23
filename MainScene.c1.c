#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

#include "Init.h"
#include "CustomConsoleIO.h"
#include "GameUtility.h"

void MainScene()
{
	int repeat_intro		= 1;
	int menu_selected		= 0;

	char title_text[]		= " Console Dungeon ";
	char start_text[]		= " Continue ";
	char newgame_text[]		= " New Game ";
	char quit_text[]		= " Quit ";

	int title_text_len		= strlen(title_text);
	int start_text_len		= strlen(start_text);
	int newgame_text_len	= strlen(newgame_text);
	int quit_text_len		= strlen(quit_text);

	float y_move_amount = 0;
	float y2_move_amount = 4.0f;

	int back_ground_tile[8][8] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,1,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
	};
	int back_ground_tile2[8][8] = {
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
	};

	while (repeat_intro)
	{
		BufferClear();

		if (_kbhit())
		{
			int keyInput = _getch();
			if (keyInput == 224)
			{
				keyInput = _getch();
				if (keyInput == 72)
				{
					if (menu_selected > 0) --menu_selected;
				}
				else if (keyInput == 80)
				{
					if (menu_selected < 2) ++menu_selected;
				}
			}
			else if (keyInput == 13)
			{
				switch (menu_selected)
				{
				case 0:
					repeat_intro = 0;
					break;
				case 1:
					repeat_intro = 0;
					break;
				case 2:
					exit(0);
					break;
				}
			}
		}

		COORD cur;

		char str_fps[30];
		float dTime = deltaTime();
		sprintf(str_fps, "FPS : %.2f", 1.0f / dTime);
		SetConsoleTitleA(str_fps);

		y_move_amount += (dTime*12.0f);
		y2_move_amount += (dTime*15.0f);
		if (y_move_amount > 8.0f) y_move_amount = fmodf(y_move_amount, 8.0f);
		if (y2_move_amount > 8.0f) y_move_amount = fmodf(y_move_amount, 8.0f);

		for (int i = 0; i < CONSOLE_WIDTH * 0.75f; ++i)
		{
			for (int j = 0; j < CONSOLE_HEIGHT; ++j)
			{
				int tile_x = (int)((float)i + (float)y_move_amount) % 8;
				int tile_y = (int)((float)j + (float)y_move_amount) % 8;

				if(back_ground_tile[tile_y][tile_x]) printXY(i * 2, j, 0, 11, "бр", 2);
				
				tile_x = (int)((float)i + (float)y2_move_amount) % 8;
				tile_y = (int)((float)j + (float)y2_move_amount) % 8;

				if (back_ground_tile2[tile_y][tile_x]) printXY(i * 2, j, 0, 12, "бс", 2);
			}
		}

		cur.X = (SHORT)((CONSOLE_WIDTH * 1.9f - title_text_len));
		cur.Y = (SHORT)(CONSOLE_HEIGHT * 0.3f);
		printXY(cur.X, cur.Y, 0, 15, title_text, title_text_len);


		cur.X = (SHORT)((CONSOLE_WIDTH * 1.9f - start_text_len) );
		cur.Y = (SHORT)(CONSOLE_HEIGHT * 0.5f);

		if(menu_selected == 0) printXY(cur.X, cur.Y, 11, 0, start_text, start_text_len);
		else printXY(cur.X, cur.Y, 0, 11, start_text, start_text_len);

		cur.X = (SHORT)((CONSOLE_WIDTH * 1.9f - newgame_text_len) );
		cur.Y = (SHORT)(CONSOLE_HEIGHT * 0.6f);

		if (menu_selected == 1) printXY(cur.X, cur.Y, 10, 0, newgame_text, newgame_text_len);
		else printXY(cur.X, cur.Y, 0, 10, newgame_text, newgame_text_len);
		
		cur.X = (SHORT)((CONSOLE_WIDTH * 1.9f - quit_text_len) );
		cur.Y = (SHORT)(CONSOLE_HEIGHT * 0.7f);

		if (menu_selected == 2) printXY(cur.X, cur.Y, 4, 0, quit_text, quit_text_len);
		else printXY(cur.X, cur.Y, 0, 4, quit_text, quit_text_len);

		BufferFlip();
	}
}