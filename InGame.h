#pragma once
#include "MakeRoom.h"

void InGame();

enum enemy_id
{
	rat,
	snake,

	mage,
	crab,

};

typedef struct Enemy {
	char * name;
	int hp;
	int max_hp;
	int atk;
	int def;
	int level;
	pos enemy_pos;
	char * icon;
} Enemy;

typedef struct EnemyList {
	Enemy * enemy;
	struct EnemyList * next;
} EnemyList;

typedef struct Level {
	size map_size;
	int ** terrain_map;
	int ** light_map;
	EnemyList * enemy_list;
	pos up_stair_pos;
	pos down_stair_pos;
} Level;

typedef struct LevelElement {
	Level * level;
	int floor;
	struct LevelElement * prevLevel;
	struct LevelElement * nextLevel;
} LevelElement;

typedef struct posQueue {
	struct posQueue * prev;
	struct posQueue * next;
	pos value;
} posQueue;