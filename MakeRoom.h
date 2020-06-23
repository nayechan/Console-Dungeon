#pragma once

#define ABS(X) (((X)<0)?-(X):(X))

typedef struct pos {
	int x;
	int y;
} pos;

typedef struct size {
	int x_size;
	int y_size;
} size;

enum TileType {
	tile_none,
	tile_wall,
	tile_decorated_wall,
	tile_floor,
	tile_decorated_floor,
	tile_water,
	tile_lava,
	tile_stair_up,
	tile_stair_down,
};

void makeRoom(int ** data, int x_size, int y_size, int createAttempt, int floor, pos * upstair_pos, pos * downstair_pos);
int isWalkable(int ** data, int x, int y); 
int isMobWalkable(int ** data, int x, int y);

int intersect(pos A, size szA, pos B, size szB);