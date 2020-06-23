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
	char start_text[]		= " Start ";
	char quit_text[]		= " Quit ";

	int title_text_len		= strlen(title_text);
	int start_text_len		= strlen(start_text);
	int quit_text_len		= strlen(quit_text);

	float y_move_amount = 0;
	float y2_move_amount = 4.0f;

	int back_ground_tile[8][8] = {
		1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1
	};
	int back_ground_tile2[8][8] = {
		1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1
	};

	while (repeat_intro)
	{
		BufferClear();

		if (_GetKeyState(VK_DOWN) == 1)
		{
			if (menu_selected < 1) ++menu_selected;
		}
		else if (_GetKeyState(VK_UP) == 1)
		{
			if (menu_selected > 0) --menu_selected;
		}
		else if (_GetKeyState(VK_RETURN) == 1)
		{
			switch (menu_selected)
			{
			case 0:
				repeat_intro = 0;
				break;
			case 1:
				exit(0);
				break;
			}
		}

		COORD cur;

		float dTime = deltaTime();

		y_move_amount += (dTime*30.0f);
		y2_move_amount += (dTime*24.0f);
		if (y_move_amount > 8.0f) y_move_amount = fmodf(y_move_amount, 8.0f);
		if (y2_move_amount > 8.0f) y_move_amount = fmodf(y_move_amount, 8.0f);

		for (int i = 0; i < CONSOLE_WIDTH * 0.75f; ++i)
		{
			for (int j = 0; j < CONSOLE_HEIGHT; ++j)
			{
				int tile_x = (int)((float)i + (float)y_move_amount) % 8;
				int tile_y = (int)((float)j + (float)y_move_amount) % 8;

				if(back_ground_tile[tile_y][tile_x]) printXY(i * 2, j, 8, 0, "  ", 2);
				
				tile_x = (int)((float)i + (float)y2_move_amount) % 8;
				tile_y = (int)((float)j + (float)y2_move_amount) % 8;

				if (back_ground_tile2[tile_y][tile_x]) printXY(i * 2, j, 7, 0, "  ", 2);
			}
		}

		cur.X = (SHORT)((CONSOLE_WIDTH * 1.9f - title_text_len));
		cur.Y = (SHORT)(CONSOLE_HEIGHT * 0.3f);
		printXY(cur.X, cur.Y, 0, 15, title_text, title_text_len);


		cur.X = (SHORT)((CONSOLE_WIDTH * 1.9f - start_text_len) );
		cur.Y = (SHORT)(CONSOLE_HEIGHT * 0.5f);

		if(menu_selected == 0) printXY(cur.X, cur.Y, 11, 0, start_text, start_text_len);
		else printXY(cur.X, cur.Y, 0, 11, start_text, start_text_len);
		
		cur.X = (SHORT)((CONSOLE_WIDTH * 1.9f - quit_text_len) );
		cur.Y = (SHORT)(CONSOLE_HEIGHT * 0.65f);

		if (menu_selected == 1) printXY(cur.X, cur.Y, 4, 0, quit_text, quit_text_len);
		else printXY(cur.X, cur.Y, 0, 4, quit_text, quit_text_len);
		char * name = "made by 1971173 ³ª¿¹Âù";
		printXY((SHORT)(CONSOLE_WIDTH * 1.95f - strlen(name)), (SHORT)(CONSOLE_HEIGHT * 0.9f), 0, 3, name,strlen(name));

		BufferFlip();
	}
}