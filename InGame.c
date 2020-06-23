#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <Windows.h>

#include "Init.h"
#include "CustomConsoleIO.h"
#include "GameUtility.h"
#include "MakeRoom.h"
#include "InGame.h"

void EnlightMap(int ** light_map, size map_size, pos player_pos)
{
	for (int x = player_pos.x - 10; x <= player_pos.x + 10; ++x)
	{
		if (x >= map_size.x_size || x<0) continue;
		for (int y = player_pos.y - 10; y <= player_pos.y + 10; ++y)
		{
			if (y >= map_size.y_size || y<0) continue;

			int dx = player_pos.x - x;
			int dy = player_pos.y - y;

			if ((dx*dx) + (dy*dy) <= 100)
				light_map[y][x] = 1;
		}
	}
}

LevelElement * InitLevelList()
{
	LevelElement * newElement = (LevelElement *)malloc(sizeof(LevelElement));
	newElement->floor = 0;
	newElement->level = NULL;
	newElement->prevLevel = NULL;
	newElement->nextLevel = NULL;

	return newElement;

}

EnemyList * InitEnemyList()
{
	EnemyList * enemyList = (EnemyList *)malloc(sizeof(EnemyList));
	enemyList->enemy = NULL;
	enemyList->next = NULL;
	return enemyList;
}

pos getEmptySpace(EnemyList * enemyList, Level * level)
{
	int x;
	int y;
	while (1)
	{
		x = rand() % level->map_size.x_size;
		y = rand() % level->map_size.y_size;
		if (level->terrain_map[y][x] != tile_floor) continue;
		int pass = 1;
		while (enemyList && enemyList->enemy)
		{
			pos enemyPos = enemyList->enemy->enemy_pos;
			if (enemyPos.x == x && enemyPos.y == y) {
				pass = 0;
				break;
			}
			enemyList = enemyList->next;
		}
		if (!pass) continue;

		break;
	}
	pos result_pos = { x,y };
	return result_pos;
}

void AddEnemy(EnemyList * enemyList, Level * level, int floor)
{
	EnemyList * itr = enemyList;
	if (itr->enemy)
	{
		while (itr->next)
		{
			itr = itr->next;
		}
		itr->next = (EnemyList *)malloc(sizeof(EnemyList));
		itr = itr->next;
	}

	itr->next = NULL;
	itr->enemy = (Enemy *)malloc(sizeof(Enemy));

	if (floor <= 10)
	{
		int enemy = rand() % 2;

		switch (enemy)
		{
		case rat:
			itr->enemy->atk = 4 + floor;
			itr->enemy->def = 2 + floor;
			itr->enemy->enemy_pos.x = -1;
			itr->enemy->enemy_pos.y = -1;
			itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
			itr->enemy->max_hp = 15 + floor;
			itr->enemy->hp = itr->enemy->max_hp;
			itr->enemy->level = floor + rand()%3;
			itr->enemy->name = "Rat";
			itr->enemy->icon = "r*";
			break;
		case snake:
			itr->enemy->atk = 2 + floor;
			itr->enemy->def = 5 + floor;
			itr->enemy->enemy_pos.x = -1;
			itr->enemy->enemy_pos.y = -1;
			itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
			itr->enemy->max_hp = 10 + floor;
			itr->enemy->hp = itr->enemy->max_hp;
			itr->enemy->level = floor + rand() % 3 + 2;
			itr->enemy->name = "Snake";
			itr->enemy->icon = "s~";
			break;
		}
	}
	else
	{
		int enemy = rand() % 4;
		switch (enemy)
		{
		case rat:
			itr->enemy->atk = 7 + floor;
			itr->enemy->def = 5 + floor;
			itr->enemy->enemy_pos.x = -1;
			itr->enemy->enemy_pos.y = -1;
			itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
			itr->enemy->max_hp = 15 + floor;
			itr->enemy->hp = itr->enemy->max_hp;
			itr->enemy->level = floor + rand() % 3;
			itr->enemy->name = "Rat";
			itr->enemy->icon = "r*";
			break;
		case snake:
			itr->enemy->atk = 5 + floor;
			itr->enemy->def = 7 + floor;
			itr->enemy->enemy_pos.x = -1;
			itr->enemy->enemy_pos.y = -1;
			itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
			itr->enemy->max_hp = 10 + floor;
			itr->enemy->hp = itr->enemy->max_hp;
			itr->enemy->level = floor + rand() % 3 + 2;
			itr->enemy->name = "Snake";
			itr->enemy->icon = "s~";
			break;
		case mage:
			itr->enemy->atk = 10 + floor;
			itr->enemy->def = floor;
			itr->enemy->enemy_pos.x = -1;
			itr->enemy->enemy_pos.y = -1;
			itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
			itr->enemy->max_hp = 2 + floor;
			itr->enemy->hp = itr->enemy->max_hp;
			itr->enemy->level = floor + rand() % 4 + 2;
			itr->enemy->name = "Melee Mage";
			itr->enemy->icon = "||";
			break;
		case crab:
			itr->enemy->atk = 8 + floor;
			itr->enemy->def = floor*2;
			itr->enemy->enemy_pos.x = -1;
			itr->enemy->enemy_pos.y = -1;
			itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
			itr->enemy->enemy_pos;
			itr->enemy->max_hp = 10 + floor*2;
			itr->enemy->hp = itr->enemy->max_hp;
			itr->enemy->level = floor + rand() % 5 + 2;
			itr->enemy->name = "Crab";
			itr->enemy->icon = "><";
			break;
		}
	}

}

void AddEnemyManual(EnemyList * enemyList, Level * level, int mobID)
{
	EnemyList * itr = enemyList;
	if (itr->enemy)
	{
		while (itr->next)
		{
			itr = itr->next;
		}
		itr->next = (EnemyList *)malloc(sizeof(EnemyList));
		itr = itr->next;
	}

	itr->next = NULL;
	itr->enemy = (Enemy *)malloc(sizeof(Enemy));

	switch (mobID)
	{
	case rat:
		itr->enemy->atk = 12;
		itr->enemy->def = 10;
		itr->enemy->enemy_pos.x = -1;
		itr->enemy->enemy_pos.y = -1;
		itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
		itr->enemy->max_hp = 20;
		itr->enemy->hp = itr->enemy->max_hp;
		itr->enemy->level = 5 + rand() % 3;
		itr->enemy->name = "Rat";
		itr->enemy->icon = "r*";
		break;
	case snake:
		itr->enemy->atk = 10;
		itr->enemy->def = 12;
		itr->enemy->enemy_pos.x = -1;
		itr->enemy->enemy_pos.y = -1;
		itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
		itr->enemy->max_hp = 15;
		itr->enemy->hp = itr->enemy->max_hp;
		itr->enemy->level = rand() % 3 + 7;
		itr->enemy->name = "Snake";
		itr->enemy->icon = "s~";
		break;
	case mage:
		itr->enemy->atk = 15;
		itr->enemy->def = floor;
		itr->enemy->enemy_pos.x = -1;
		itr->enemy->enemy_pos.y = -1;
		itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
		itr->enemy->max_hp = 13;
		itr->enemy->hp = itr->enemy->max_hp;
		itr->enemy->level = rand() % 4 + 7;
		itr->enemy->name = "Melee Mage";
		itr->enemy->icon = "||";
		break;
	case crab:
		itr->enemy->atk = 13;
		itr->enemy->def = 10;
		itr->enemy->enemy_pos.x = -1;
		itr->enemy->enemy_pos.y = -1;
		itr->enemy->enemy_pos = getEmptySpace(enemyList, level);
		itr->enemy->max_hp = 20;
		itr->enemy->hp = itr->enemy->max_hp;
		itr->enemy->level = rand() % 5 + 7;
		itr->enemy->name = "Crab";
		itr->enemy->icon = "><";
		break;
	}
}


posQueue * InitPosQueue()
{
	posQueue * queue = (posQueue *)malloc(sizeof(posQueue));
	queue->prev = NULL;
	queue->next = NULL;
	queue->value.x = -1;
	queue->value.y = -1;
	return queue;
}

void pushQueue(posQueue * queue, pos value)
{
	posQueue * queueItr = queue;
	if (queue->value.x == -1 && queue->value.y == -1)
	{
		queue->value = value;
	}
	else
	{
		while (queueItr->next)
		{
			queueItr = queueItr->next;
		}
		queueItr->next = (posQueue *)malloc(sizeof(posQueue));
		queueItr->next->prev = queueItr;
		queueItr->next->next = NULL;
		queueItr->next->value = value;
	}
}

posQueue * popQueue(posQueue * queue)
{
	if (queue->next)
	{
		posQueue * tmp = queue;
		queue = queue->next;
		queue->prev = NULL;
		free(tmp);
	}
	else
	{
		queue->value.x = -1;
		queue->value.y = -1;
	}
	return queue;
}

void removeQueue(posQueue * queue)
{
	while (queue)
	{
		posQueue * tmp = queue->next;
		free(queue);
		queue = tmp;
	}
}

EnemyList * DestroyMob(Enemy * mob, EnemyList * list)
{
	EnemyList * itrList = list;
	if (mob == list->enemy)
	{
		list = list->next;
		free(itrList->enemy);
		free(itrList);
		itrList = list;
	}
	else
	{
		while (itrList->next->enemy != mob)
		{
			itrList = itrList->next;
		}
		EnemyList * toDelete = itrList->next;
		itrList->next = itrList->next->next;
		free(toDelete->enemy);
		free(toDelete);
		itrList = list;
	}
	return itrList;
}


void printInfoText(char * str, int mode)
{
	static int line = 0;
	static char _screenBuffer[3][CONSOLE_WIDTH * 2 + 1];
	if (mode)
	{

		if (line >= 3)
		{
			strcpy(_screenBuffer[0], _screenBuffer[1]);
			strcpy(_screenBuffer[1], _screenBuffer[2]);
			strcpy(_screenBuffer[2], str);
			line = 2;
		}
		else strcpy(_screenBuffer[line], str);

		for (int i = 0; i <= line; ++i)
		{
			printXY(0, i, 0, 15, _screenBuffer[i], strlen(_screenBuffer[i]));
		}
		++line;
	}
	else
	{
		for (int i = 0; i <= 3; ++i)
		{
			printXY(0, i, 0, 15, _screenBuffer[i], strlen(_screenBuffer[i]));
		}
	}
	
}
void gameOver()
{
	BufferClear();
	BufferFlip();
	BufferClear();
	BufferFlip();
	
	printInfoText("You Die.", 1);
	char * game_over_text = "<< Game Over >>";
	char * press_any_key_text = "  Press Enter to exit... ";
	printXY((CONSOLE_WIDTH)-(strlen(game_over_text)/2),15, 4, 15, game_over_text, strlen(game_over_text));
	printXY((CONSOLE_WIDTH) - (strlen(press_any_key_text) / 2), 20, 4, 15, press_any_key_text, strlen(press_any_key_text));
	BufferFlip();
	while (_GetKeyState(VK_RETURN) != 1)
	{
	}
	exit(0);
}

void TakeDamage(int defence, int level, int *hp, int mob_damage, int mob_level, char * mob_name)
{
	float bonus = 1.0f;

	if (mob_level - level < -5)
		bonus = 0.8f;
	else if (mob_level - level < -2)
		bonus = 0.9f;
	else if (mob_level - level > 2)
		bonus = 1.2f;
	else if (mob_level - level > 5)
		bonus = 1.5f;
	else
		bonus = 5.0f;

	int total_damage = (mob_damage - defence);
	if (total_damage < 3) total_damage = rand() % 3;

	total_damage = (int)((float)total_damage * (bonus - ((float)(rand() % 10) / 100.0f)));
	if (total_damage < 3) total_damage = rand() % 3;

	*hp -= total_damage;

	char str[CONSOLE_WIDTH * 2 + 1];
	sprintf(str, "You took %d damage from %s.", total_damage, mob_name);
	printInfoText(str, 1);

	if (*hp < 0) gameOver();
}


void MoveEnemys(Level * level, pos player_pos, int player_def, int player_level, int * player_hp)
{
	EnemyList * itr = level->enemy_list;
	if (itr && itr->enemy)
	{
		while (itr)
		{
			if (ABS(itr->enemy->enemy_pos.x - player_pos.x)
				+ABS(itr->enemy->enemy_pos.y - player_pos.y)
				>40)
			{
				itr = itr->next;
				continue;
			}
			int not_available = 0;
			posQueue * queue = InitPosQueue();
			int dx[4] = { 0,-1,1,0 };
			int dy[4] = { -1,0,0,1 };

			int ** accessed = (int **)malloc(sizeof(int*) * level->map_size.y_size);
			for (int i = 0; i < level->map_size.y_size; ++i)
			{
				accessed[i] = (int *)malloc(sizeof(int)*level->map_size.x_size);
				for (int j = 0; j < level->map_size.x_size; ++j)
				{
					accessed[i][j] = -1;
				}
			}

			int oldCount = 1;
			accessed[itr->enemy->enemy_pos.y][itr->enemy->enemy_pos.x] = 0;
			pushQueue(queue, itr->enemy->enemy_pos);

			int _level = 1;
			while (1)
			{
				posQueue * itrQueue = queue;
				int flag = 0;

				int newCount = 0;
				for(int i=0;i<oldCount;++i)
				{
					for (int i = 0; i < 4; ++i)
					{
						pos newPos = { itrQueue->value.x + dx[i], itrQueue->value.y + dy[i] };
						if (!(newPos.x>=0 && newPos.x<level->map_size.x_size && newPos.y>=0 && newPos.y < level->map_size.y_size) ||
							accessed[newPos.y][newPos.x] != -1 || !isMobWalkable(level->terrain_map, newPos.x, newPos.y ))
						{
							continue;
						}
						else
						{
							int cont = 0;
							EnemyList * iitr = level->enemy_list;
							while (iitr)
							{
								if (newPos.x == iitr->enemy->enemy_pos.x &&
									newPos.y == iitr->enemy->enemy_pos.y)
								{
									cont = 1;
									break;
								}
								iitr = iitr->next;
							}
							if (cont) continue;
						}
						accessed[newPos.y][newPos.x] = _level;
						if (newPos.x == player_pos.x && newPos.y == player_pos.y)
							flag = 1;
						if (flag) break;
						pushQueue(queue, newPos);
						++newCount;
					}

					if (flag) break;

					itrQueue = itrQueue->next;
				}
				if (flag) break;
				for (int i = 0; i < oldCount; ++i)
				{
					queue = popQueue(queue);
				}
				if (queue->value.x == -1 && queue->value.y == -1)
				{
					not_available = 1;
					removeQueue(queue);
					break;
				}
				oldCount = newCount;

				++_level;
			}
			if (not_available) {
				itr = itr->next;
				for (int i = 0; i < level->map_size.y_size; ++i)
				{
					free(accessed[i]);
				}
				free(accessed);
				continue;
			}
			removeQueue(queue);

			int x = player_pos.x;
			int y = player_pos.y;

			while (accessed[y][x] > 1)
			{
				int dx[4] = { 0,-1,1,0 };
				int dy[4] = { -1,0,0,1 };
				for (int i = 0; i < 4; ++i)
				{
					if (accessed[y][x] - 1 == accessed[y+dy[i]][x+dx[i]])
					{
						x += dx[i];
						y += dy[i];
						break;
					}
				}
			}

			if (!not_available)
			{
				if (player_pos.x == x && player_pos.y == y)
				{
					TakeDamage(player_def, player_level, player_hp, itr->enemy->atk, itr->enemy->level, itr->enemy->name);
				}
				else
				{
					itr->enemy->enemy_pos.x = x;
					itr->enemy->enemy_pos.y = y;

				}
			}

			for (int i = 0; i < level->map_size.y_size; ++i)
			{
				free(accessed[i]);
			}
			free(accessed);

			itr = itr->next;
		}
	}
}

void AddLevel(LevelElement * level, int map_size_x, int map_size_y, int frequency, int mob_count)
{
	size map_size = { map_size_x, map_size_y };
	while (level->nextLevel)
	{
		level = level->nextLevel;
	}

	if (level->floor)
	{
		level->nextLevel = (LevelElement *)malloc(sizeof(LevelElement));
		level->nextLevel->prevLevel = level;
		level->nextLevel->floor = level->floor + 1;
		level = level->nextLevel;
	}
	else
		level->floor = 1;

	level->level = (Level *)malloc(sizeof(Level));

	level->nextLevel = NULL;

	level->level->map_size = map_size;

	level->level->terrain_map	= (int **)malloc(sizeof(int *)*map_size_y);
	level->level->light_map		= (int **)malloc(sizeof(int *)*map_size_y);

	for (int i = 0; i < map_size_y; ++i)
	{
		level->level->terrain_map[i]	= (int *)malloc(sizeof(int)*map_size_x);
		level->level->light_map[i]		= (int *)malloc(sizeof(int)*map_size_x);

		for (int j = 0; j < map_size_x; ++j)
		{
			level->level->terrain_map[i][j] = 0;
			level->level->light_map[i][j] = 0;
		}
	}
	makeRoom(level->level->terrain_map, map_size_x, map_size_y, frequency, level->floor, &level->level->up_stair_pos, &level->level->down_stair_pos);

	level->level->enemy_list = InitEnemyList();

	int mobFreq = mob_count + rand() % 5;
	for(int i=0;i<mobFreq;++i)
		AddEnemy(level->level->enemy_list, level->level, level->floor);
}

Enemy * getObjectMob(EnemyList * enemy_list, int x, int y)
{
	EnemyList * itrEnemy = enemy_list;
	while (itrEnemy)
	{
		if (itrEnemy->enemy->enemy_pos.x == x &&
			itrEnemy->enemy->enemy_pos.y == y)
		{
			return itrEnemy->enemy;
		}
		itrEnemy = itrEnemy->next;
	}
	return NULL;
}

EnemyList * dealDamageToMob(int attack, int level, Enemy * mob, EnemyList * list, int * killed, int * dealt_damage)
{
	float bonus = 1.0f;
	int mob_level = mob->level;
	int mob_defence = mob->def;

	if (mob_level - level < -5)
		bonus = 1.2f;
	else if (mob_level - level < -2)
		bonus = 1.1f;
	else if (mob_level - level > 2)
		bonus = 0.8f;
	else if (mob_level - level > 5)
		bonus = 0.5f;
	else 
		bonus = 0.2f;

	int total_damage = (attack - mob_defence);
	if (total_damage < 3) total_damage = rand()%3;

	total_damage = (int)((float)total_damage * (bonus+((float)(rand()%10)/100.0f)));
	if (total_damage < 3) total_damage = rand()%3+2;

	mob->hp -= total_damage;
	if (mob->hp < 0)
	{
		list = DestroyMob(mob, list);
		*killed = 1;
	}
	*dealt_damage = total_damage;
	return list;
}

void InGame()
{
	pos player_pos;
	LevelElement * level_list = InitLevelList();

	AddLevel(level_list, 32+rand()%32, 32+rand()%32, 50+rand()%50, 33+rand()%16);

	char game_hp_text[CONSOLE_WIDTH+1];
	char game_stat_text[CONSOLE_WIDTH * 2 + 1];

	int level = 1;

	int max_hp = 100 + level * 20;
	int hp = max_hp;

	long long current_exp = 0;
	long long max_exp = floor(pow((double)level, 2.3f)) + (level*23);

	float attack_buff_amount = 1.0f;
	float defence_buff_amount = 1.0f;

	int weapon_attack = 0;
	int armor_defence = 0;

	int attack = (int)(attack_buff_amount * (float)(5 + (level * 2) + weapon_attack));
	int defence = (int)(defence_buff_amount * (float)(3 + level + armor_defence));

	int healing_potion = 3;
	int experience_potion = 0;
	int strength_potion = 0;
	int defence_potion = 0;
	int unknown_potion = 0;

	printInfoText("Welcome to Console Dungeon!", 1);


	for (int i = 0; i < level_list->level->map_size.y_size; ++i)
	{
		for (int j = 0; j < level_list->level->map_size.x_size; ++j)
		{
			if (level_list->level->terrain_map[i][j] == tile_stair_up)
			{
				player_pos.x = j;
				player_pos.y = i + 1;
			}
		}
	}


	while (1)
	{
		BufferClear();

		float dTime = deltaTime();

		Enemy * objectMob = NULL;

		int moved = 0;

		if (_GetKeyState(VK_LEFT) == 1)
		{
			objectMob = getObjectMob(level_list->level->enemy_list, player_pos.x - 1, player_pos.y);
			if (isWalkable(level_list->level->terrain_map, player_pos.x - 1, player_pos.y) && !objectMob)
				--player_pos.x;
			moved = 1;
		}
		else if (_GetKeyState(VK_RIGHT) == 1)
		{
			objectMob = getObjectMob(level_list->level->enemy_list, player_pos.x + 1, player_pos.y);
			if (isWalkable(level_list->level->terrain_map, player_pos.x + 1, player_pos.y) && !objectMob)
				++player_pos.x;
			moved = 1;
		}
		else if(_GetKeyState(VK_UP) == 1)
		{
			objectMob = getObjectMob(level_list->level->enemy_list, player_pos.x, player_pos.y - 1);
			if (isWalkable(level_list->level->terrain_map, player_pos.x, player_pos.y - 1) && !objectMob)
				--player_pos.y;
			moved = 1;
		}
		else if(_GetKeyState(VK_DOWN) == 1)
		{
			objectMob = getObjectMob(level_list->level->enemy_list, player_pos.x, player_pos.y + 1);
			if (isWalkable(level_list->level->terrain_map, player_pos.x, player_pos.y + 1) && !objectMob)
				++player_pos.y;
			moved = 1;
		}
		else if (_GetKeyState(VK_SPACE) == 1)
		{
			MoveEnemys(level_list->level, player_pos, defence, level, &hp);
			moved = 1;
		}
		if (moved)
		{

			MoveEnemys(level_list->level, player_pos, defence, level, &hp);
			if (level_list->level->terrain_map[player_pos.y][player_pos.x] == tile_lava)
				TakeDamage(defence, level, &hp, 5, level, "Lava");
			else if (level_list->level->terrain_map[player_pos.y][player_pos.x] == tile_water)
			{
				++hp;
				if (hp > max_hp) hp = max_hp;
			}

			if (objectMob)
			{
				int killed = 0;
				int dealt_damage = 0;
				int mob_exp = objectMob->level * 3 + rand() % objectMob->level;
				int mob_level = objectMob->level;
				char * mob_name = objectMob->name;
				level_list->level->enemy_list = dealDamageToMob(attack, level, objectMob, level_list->level->enemy_list, &killed, &dealt_damage);
				if (killed)
				{
					current_exp += mob_exp;
					if (current_exp > max_exp)
					{
						++level; 
						max_hp = 100 + level * 20;
						hp = max_hp;

						current_exp = 0;
						max_exp = floor(pow((double)level, 2.3f)) + (level * 23);

						attack_buff_amount = 1.0f;
						defence_buff_amount = 1.0f;

						attack = (int)(attack_buff_amount * (float)(5 + (level * 2) + weapon_attack));
						defence = (int)(defence_buff_amount * (float)(3 + level + armor_defence));
					}

					if (rand() % 5 == 0) ++healing_potion;
					if (mob_level >= 3)
					{
						if(rand()%20 == 0) ++experience_potion;
					}
					if (mob_level >= 5)
					{
						if (rand() % 10 == 0) ++strength_potion;
					}
					if (mob_level >= 5)
					{
						if (rand() % 10 == 0) ++defence_potion;
					}
					if (mob_level >= 8)
					{
						if (rand() % 50 == 0) ++unknown_potion;
					}
					char str[CONSOLE_WIDTH * 2 + 1];
					sprintf(str, "You kill %s with %d damage.", mob_name, dealt_damage);
					printInfoText(str, 1);
				}
				else
				{
					char str[CONSOLE_WIDTH * 2 + 1];
					sprintf(str, "You hit %s with %d damage.", mob_name, dealt_damage);
					printInfoText(str, 1);
				}
			}
		}

		if (_GetKeyState(0x41 + 'q' - 'a') == 1)
		{
			if (healing_potion)
			{
				--healing_potion;
				hp = max_hp;
			}
		}
		if (_GetKeyState(0x41 + 'w' - 'a') == 1)
		{
			if (experience_potion)
			{
				--experience_potion;

				++level;
				max_hp = 100 + level * 20;
				hp = max_hp;

				current_exp = 0;
				max_exp = floor(pow((double)level, 2.3f)) + (level * 23);

				attack_buff_amount = 1.0f;
				defence_buff_amount = 1.0f;

				attack = (int)(attack_buff_amount * (float)(5 + (level * 2) + weapon_attack));
				defence = (int)(defence_buff_amount * (float)(3 + level + armor_defence));
			}
		}
		if (_GetKeyState(0x41 + 'e' - 'a') == 1)
		{
			if (strength_potion)
			{
				--strength_potion;
				weapon_attack += rand() % 4 + 3;
				attack = (int)(attack_buff_amount * (float)(5 + (level * 2) + weapon_attack));
			}
		}
		if (_GetKeyState(0x41 + 'r' - 'a') == 1)
		{
			if (defence_potion)
			{
				--defence_potion;
				armor_defence += rand() % 2 + 2;
				defence = (int)(defence_buff_amount * (float)(3 + level + armor_defence));
			}
		}
		if (_GetKeyState(0x41 + 't' - 'a') == 1)
		{
			if (unknown_potion)
			{
				--unknown_potion;
				weapon_attack += (-10+rand()%30);
				if (weapon_attack < 0) weapon_attack = 0;
				attack = (int)(attack_buff_amount * (float)(5 + (level * 2) + weapon_attack)); 
				armor_defence += (-10 + rand() % 30);
				if (armor_defence < 0) armor_defence = 0;
				defence = (int)(defence_buff_amount * (float)(3 + level + armor_defence));
				int dice = rand() % 2;
				if(dice) hp = 1;
				else hp = max_hp;
			}
		}
		if (_GetKeyState(VK_RETURN) == 1)
		{
			if (level_list->level->terrain_map[player_pos.y][player_pos.x] == tile_stair_down)
			{
				char str[CONSOLE_WIDTH * 2 + 1];
				sprintf(str, "You descended to floor %d.", level_list->floor+1);
				printInfoText(str, 1);
				if (level_list->nextLevel)
				{
					level_list = level_list->nextLevel;
					player_pos.x = level_list->level->up_stair_pos.x+1;
					player_pos.y = level_list->level->up_stair_pos.y+2;
				}
				else
				{
					AddLevel(level_list, rand() % 64 + 64, rand()%64 + 64, rand() % 40 + 20, rand() % 20 + 20);
					level_list = level_list->nextLevel;
					player_pos.x = level_list->level->up_stair_pos.x+1;
					player_pos.y = level_list->level->up_stair_pos.y+2;
				}

			}
			else if (level_list->level->terrain_map[player_pos.y][player_pos.x] == tile_stair_up)
			{
				char str[CONSOLE_WIDTH * 2 + 1];
				sprintf(str, "You ascended to floor %d.", level_list->floor - 1);
				printInfoText(str, 1);
				if (level_list->prevLevel)
				{
					level_list = level_list->prevLevel;
					player_pos.x = level_list->level->down_stair_pos.x+1;
					player_pos.y = level_list->level->down_stair_pos.y+2;
				}
			}
		}

		sprintf(game_hp_text, "HP : %d / %d", hp, max_hp);
		sprintf(
			game_stat_text,
			"LV. %d EXP %d / %d (%.2f%%) ATK %d DEF %d",
			level,
			current_exp,
			max_exp,
			(float)current_exp / (float)max_exp*100.0f,
			attack,
			defence
		);

		EnlightMap(level_list->level->light_map, level_list->level->map_size, player_pos);

		for (int i = 0; i < CONSOLE_HEIGHT; ++i)
		{
			for (int j = 0; j < CONSOLE_WIDTH; ++j)
			{
				int dx = player_pos.x - (CONSOLE_WIDTH / 2);
				int dy = player_pos.y - (CONSOLE_HEIGHT / 2);

				if (j + dx >= level_list->level->map_size.x_size || j + dx < 0 || i + dy >= level_list->level->map_size.y_size || i + dy < 0)
					continue;



				int d_x = (CONSOLE_WIDTH / 2)	- j;
				int d_y = (CONSOLE_HEIGHT / 2)	- i;
				int is_lighten = 0;

				if ((d_x*d_x) + (d_y*d_y) <= 64)
					is_lighten = 1;

				if (level_list->level->light_map[i + dy][j + dx])
				{
					switch (level_list->level->terrain_map[i + dy][j + dx])
					{
					case tile_floor:
						if(is_lighten)
							printXY(j * 2, i, 7, 0, "  ", 2);
						else
							printXY(j * 2, i, 0, 7, "¢Æ", 2);
						break;
					case tile_water:
						if (is_lighten)
							printXY(j * 2, i, 11, 0, "  ", 2);
						else
							printXY(j * 2, i, 0, 11, "¢Æ", 2);
						break;
					case tile_lava :
						if (is_lighten)
							printXY(j * 2, i, 4, 0, "  ", 2);
						else
							printXY(j * 2, i, 0, 4, "¢Æ", 2);
						break;
					case tile_wall:
						if (is_lighten)
							printXY(j * 2, i, 7, 8, "¡á", 2);
						else
							printXY(j * 2, i, 8, 0, "¡á", 2);
						break;
					case tile_stair_up:
						if (is_lighten)
							printXY(j * 2, i, 15, 3, "¡ã", 2);
						else
							printXY(j * 2, i, 7, 3, "¡ã", 2);
						break;
					case tile_stair_down:
						if (is_lighten)
							printXY(j * 2, i, 7, 4, "¡å", 2);
						else
							printXY(j * 2, i, 15, 4, "¡å", 2);
						break;
					default:
						break;
					}
				}

				EnemyList * itr = level_list->level->enemy_list;
				if (itr && itr->enemy)
				{
					while (itr)
					{
						pos enemy_pos = itr->enemy->enemy_pos;
						if (enemy_pos.x == j + dx && enemy_pos.y == i + dy && is_lighten)
						{
							int attribute = attributeBuffer[i][j * 2];
							char ch = textBuffer[i][j * 2];
							int fg = attribute % 16;
							attribute >>= 4;
							int bg = attribute % 16;
							printXY(j * 2, i, bg, fg, itr->enemy->icon, 2);
						}
						itr = itr->next;
					}
				}

				if (player_pos.x == j + dx && player_pos.y == i + dy)
				{
					int attribute = attributeBuffer[i][j * 2];
					char ch = textBuffer[i][j * 2];
					int fg = attribute % 16;
					attribute >>= 4;
					int bg = attribute % 16;
					printXY(j * 2, i, bg, fg, "£À", 2);
				}


			}
		}

		for (int i = 0; i < 3; ++i)
		{

			for (int j = 0; j < CONSOLE_WIDTH * 2; ++j)
			{
				printXY(j, i, 0, 15, " ", 1);
			}
		}

		printInfoText(NULL, 0);

		for (int i = 0; i < CONSOLE_WIDTH * 2; ++i)
		{
			printXY(i, 3, 0, 15, "-", 1);
		}

		for (int i = 0; i < CONSOLE_WIDTH * 2; ++i)
		{
			printXY(i, CONSOLE_HEIGHT - 3, 0, 15, "-", 1);
		}
		for (int i = CONSOLE_HEIGHT-2; i < CONSOLE_HEIGHT; ++i)
		{
			for (int j = 0; j < CONSOLE_WIDTH*2; ++j)
			{
				printXY(j, i, 0, 15, " ", 1);
			}
		}
		printXY(0, CONSOLE_HEIGHT - 2, 0, 15, game_hp_text, strlen(game_hp_text));
		printXY(0, CONSOLE_HEIGHT - 1, 0, 15, game_stat_text, strlen(game_stat_text));

		for (int i = 4; i <= CONSOLE_HEIGHT-4; ++i)
		{
			printXY(CONSOLE_WIDTH*2-16, i, 0, 15, "|", 1);
		}
		for (int i = CONSOLE_WIDTH * 2 - 15; i < CONSOLE_WIDTH * 2; ++i)
		{
			for (int j = 4; j <= CONSOLE_HEIGHT - 4; ++j)
			{
				printXY(i, j, 0, 15, " ", 1);
			}
		}
		char healing_potion_info[16];
		char experience_potion_info[16];
		char strength_potion_info[16];
		char defence_potion_info[16];
		char unknown_potion_info[16];

		sprintf(healing_potion_info,	" (Q) x%d ", healing_potion);
		sprintf(experience_potion_info, " (W) x%d ", experience_potion);
		sprintf(strength_potion_info,	" (E) x%d ", strength_potion);
		sprintf(defence_potion_info,	" (R) x%d ", defence_potion);
		sprintf(unknown_potion_info,	" (T) x%d ", unknown_potion);

		printXY(CONSOLE_WIDTH * 2 - 14, 6, 4, 15,  " HP  Potion ", strlen(" HP  Potion "));
		printXY(CONSOLE_WIDTH * 2 - 14, 10, 5, 15, " EXP Potion ", strlen(" EXP Potion "));
		printXY(CONSOLE_WIDTH * 2 - 14, 14, 1, 15, " STR Potion ", strlen(" STR Potion "));
		printXY(CONSOLE_WIDTH * 2 - 14, 18, 2, 15, " DEF Potion ", strlen(" DEF Potion "));
		printXY(CONSOLE_WIDTH * 2 - 14, 22, 8, 15, " ?   Potion ", strlen(" ?   Potion "));

		printXY(CONSOLE_WIDTH * 2 - 10, 8, 4, 15, healing_potion_info, strlen(healing_potion_info));
		printXY(CONSOLE_WIDTH * 2 - 10, 12, 5, 15, experience_potion_info, strlen(experience_potion_info));
		printXY(CONSOLE_WIDTH * 2 - 10, 16, 1, 15, strength_potion_info, strlen(strength_potion_info));
		printXY(CONSOLE_WIDTH * 2 - 10, 20, 2, 15, defence_potion_info, strlen(defence_potion_info));
		printXY(CONSOLE_WIDTH * 2 - 10, 24, 8, 15, unknown_potion_info, strlen(unknown_potion_info));


		BufferFlip();
	}
}