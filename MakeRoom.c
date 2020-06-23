#include "MakeRoom.h"
#include <stdlib.h>

int intersect(pos A, size szA, pos B, size szB)
{
	return
		A.x-1 < B.x + szB.x_size && 
		A.y-1 < B.y + szB.y_size &&
		A.x + szA.x_size > B.x-1 && 
		A.y + szA.y_size > B.y-1;
}

void setTileFloor(int ** data, int x, int y)
{
	int dice = rand()%100;
	if(dice >= 90)
		data[y][x] = tile_water;

	else if (dice < 3)
		data[y][x] = tile_lava;

	else
		data[y][x] = tile_floor;
}

int isWalkable(int ** data, int x, int y)
{
	switch (data[y][x])
	{
	case tile_floor:
	case tile_water:
	case tile_lava:
	case tile_stair_up:
	case tile_stair_down:
		return 1;
	default:
		return 0;
	}
}

int isMobWalkable(int ** data, int x, int y)
{
	switch (data[y][x])
	{
	case tile_floor:
	case tile_water:
		return 1;
	default:
		return 0;
	}
}

void makeRoom(int ** data, int x_size, int y_size, int createAttempt, int floor, pos * upstair_pos, pos * downstair_pos)
{
	enum TemplateType {
		template_default_type = -1,
		template_start,
		template_end,
		template_trap_zone,
		template_sub_boss_room,
		template_boss_room_type1,
		template_boss_room_type2,
		template_floor50_boss,
		template_floor100_boss,
		template_final_boss
	};
	

	pos start, end;
	size szStart, szEnd;
	do
	{
		szStart.x_size = 3;
		szStart.y_size = 3;
		start.x = (rand() % (x_size-szStart.x_size-1))+1;
		start.y = (rand() % (y_size-szStart.y_size-1))+1;

		szEnd.x_size = 3;
		szEnd.y_size = 3;
		end.x = (rand() % (x_size-szEnd.x_size-1))+1;
		end.y = (rand() % (y_size-szEnd.y_size-1))+1;
	}
	while (
		ABS(start.x - end.x) < (int)(0.6*(float)x_size)
		||
		ABS(start.y - end.y) < (int)(0.6*(float)y_size)
		|| 
		intersect(start, szStart, end, szEnd)
		);

	for (int i = 0; i < szStart.y_size; ++i)
	{
		for (int j = 0; j < szStart.x_size; ++j)
		{
			if (i == j && i == (szStart.x_size/2))
			{
				data[i + start.y][j + start.x] = tile_stair_up;
				data[i + end.y][j + end.x] = tile_stair_down;
			}
			else
			{
				data[i + start.y][j + start.x] = tile_floor;
				data[i + end.y][j + end.x] = tile_floor;
			}
		}
	}

	pos * room = (pos *)malloc(sizeof(pos)*(createAttempt+2));
	room[0] = start;
	room[1] = end;

	*upstair_pos = room[0];
	*downstair_pos	= room[1];

	size * room_size = (size *)malloc(sizeof(size)*(createAttempt + 2));
	room_size[0] = szStart;
	room_size[1] = szEnd;

	int succeed = 2;
	for (int i = 0; i < createAttempt; ++i)
	{
		int flag = 1;
		pos p;
		size szP;
		szP.x_size = rand() % 3 + 3;
		szP.y_size = rand() % 3 + 3;
		p.x = (rand() % (x_size - szP.x_size - 1))+1;
		p.y = (rand() % (y_size - szP.y_size - 1))+1;

		for (int j = 0; j < succeed; ++j)
		{
			pos e = room[j];
			size szE = room_size[j];
			if (intersect(p, szP, e, szE))
			{
				flag = 0;
				break;
			}
		}

		if (flag)
		{
			room[succeed] = p;
			room_size[succeed] = szP;

			++succeed;

			for (int szi = 0; szi < szP.y_size; ++szi)
			{
				for (int szj = 0; szj < szP.x_size; ++szj)
				{
					data[p.y + szi][p.x + szj] = tile_floor;
				}
			}
		}
	}

	int ** distance = (int **)malloc(sizeof(int *)*succeed);
	int ** connection = (int **)malloc(sizeof(int *)*succeed);
	for (int i = 0; i < succeed; ++i)
	{
		distance[i] = (int *)malloc(sizeof(int)*succeed);
		connection[i] = (int *)malloc(sizeof(int)*succeed);
		for (int j = 0; j < succeed; ++j)
		{
			if (i == j) distance[i][j] = 0;
			else
			{
				int xaxis = (2 * room[i].x + room_size[i].x_size) - (2 * room[j].x + room_size[j].x_size);
				int yaxis = (2 * room[i].y + room_size[i].y_size) - (2 * room[j].y + room_size[j].y_size);
				xaxis *= xaxis; yaxis *= yaxis;
				distance[i][j] = xaxis + yaxis;
			}
			if(i==j) connection[i][j] = 1;
			else connection[i][j] = 0;
		}
	}
	
	for (int i = 0; i < succeed; ++i)
	{
		int nearest = -1;
		int v = -1;
		for (int j = 0; j < succeed; ++j)
		{
			if ((i != j) && (v == -1 || distance[i][j] < v) && (!connection[i][j]))
			{
				nearest = j;
				v = distance[i][j];
			}
		}

		connection[i][nearest] = 1;
		connection[nearest][i] = 1;

		int diff_x = room[nearest].x - room[i].x;
		int diff_y = room[nearest].y - room[i].y;

		if (diff_x > 0)
		{
			if (diff_y > 0)
			{
				pos pA, pB;

				pA.x = room[i].x + (rand() % room_size[i].x_size);
				pA.y = room[i].y;

				pB.x = room[nearest].x;
				pB.y = room[nearest].y + (rand() % room_size[nearest].y_size);

				int dX = pB.x - pA.x;
				int dY = pB.y - pA.y;

				for (int j = pA.y; j != pB.y; (dY > 0) ? ++j : --j)
				{
					if (data[j][pA.x] == tile_none) setTileFloor(data, pA.x, j); // data[j][pA.x] = tile_floor;
				}

				for (int j = pA.x; j != pB.x; (dX > 0) ? ++j : --j)
				{
					if (data[pB.y][j] == tile_none) setTileFloor(data, j, pB.y); // data[pB.y][j] = tile_floor;
				}

			}
			else
			{
				pos pA, pB;

				pA.x = room[i].x + room_size[i].x_size - 1;
				pA.y = room[i].y + (rand() % room_size[i].y_size);

				pB.x = room[nearest].x + (rand() % room_size[nearest].x_size);
				pB.y = room[nearest].y;

				int dX = pB.x - pA.x;
				int dY = pB.y - pA.y;

				for (int j = pA.x; j != pB.x; (dX > 0) ? ++j : --j)
				{
					if (data[pA.y][j] == tile_none) setTileFloor(data, j, pA.y);
				}

				for (int j = pA.y; j != pB.y; (dY > 0) ? ++j : --j)
				{
					if (data[j][pB.x] == tile_none) setTileFloor(data, pB.x, j);
				}
			}
		}
		else
		{
			if (diff_y > 0)
			{
				pos pA, pB;

				pA.x = room[i].x;
				pA.y = room[i].y + (rand() % room_size[i].y_size);

				pB.x = room[nearest].x + (rand() % room_size[nearest].x_size);
				pB.y = room[nearest].y + room_size[nearest].y_size - 1;

				int dX = pB.x - pA.x;
				int dY = pB.y - pA.y;

				for (int j = pA.x; j != pB.x; (dX > 0) ? ++j : --j)
				{
					if (data[pA.y][j] == tile_none) setTileFloor(data, j, pA.y);
				}

				for (int j = pA.y; j != pB.y; (dY > 0) ? ++j : --j)
				{
					if (data[j][pB.x] == tile_none) setTileFloor(data, pB.x, j);
				}
			}
			else
			{
				pos pA, pB;

				pA.x = room[i].x + (rand() % room_size[i].x_size);
				pA.y = room[i].y + room_size[i].y_size - 1;

				pB.x = room[nearest].x + room_size[nearest].x_size - 1;
				pB.y = room[nearest].y + (rand() % room_size[nearest].y_size);

				int dX = pB.x - pA.x;
				int dY = pB.y - pA.y;

				for (int j = pA.y; j != pB.y; (dY > 0) ? ++j : --j)
				{
					if (data[j][pA.x] == tile_none) setTileFloor(data, pA.x, j);
				}

				for (int j = pA.x; j != pB.x; (dX > 0) ? ++j : --j)
				{
					if (data[pB.y][j] == tile_none) setTileFloor(data, j, pB.y);
				}
			}
		}

	}

	int * queue_connection = (int *)malloc(sizeof(int)*succeed);
	int queue_index = 0;
	int queue_size = 0;

	for (int i = 0; i < succeed; ++i)
	{
		queue_connection[i] = -1;
	}

	for (int i = 0; i < succeed; ++i)
	{
		if (connection[0][i])
		{
			queue_connection[queue_size++] = i;
		}
	}

	int dSize;
	do
	{
		dSize = 0;
		int prevSize = queue_size;
		for (int i = queue_index; i < prevSize; ++i)
		{
			for (int j = 0; j < succeed; ++j)
			{
				if (connection[queue_connection[i]][j])
				{
					int isExist = 0;
					for (int k = 0; k < prevSize; ++k)
					{
						if (j == queue_connection[k])
						{
							isExist = 1;
							break;
						}
					}
					if (!isExist)
					{
						++dSize;
						queue_connection[queue_size++] = j;
					}
				}
			}
		}
		queue_index = prevSize;
	} while (dSize);

	for (int i = queue_size - 1; i > 0; --i) {
		for (int j = 0; j < i; ++j) {
			if (queue_connection[j] > queue_connection[j + 1]) {
				int tmp = queue_connection[j];
				queue_connection[j] = queue_connection[j + 1];
				queue_connection[j + 1] = tmp;
			}
		}
	}

	if (succeed - queue_size > 0)
	{
		int * need_connection = (int *)malloc(sizeof(int)*(succeed - queue_size));
		int cSize = 0;

		queue_index = 0;

		for (int i = 0; i < succeed; ++i)
		{
			if (i == queue_connection[queue_index])
			{
				++queue_index;
			}
			else
			{
				need_connection[cSize++] = i;
			}
		}

		for (int i = 0; i < cSize; ++i)
		{
			int i_index = need_connection[i];
			int nearest = -1;
			int v = -1;
			for (int j = 0; j < queue_size; ++j)
			{
				int j_index = queue_connection[j];
				if ((v == -1 || distance[i_index][j_index] < v) && (!connection[i_index][j_index]))
				{
					nearest = j_index;
					v = distance[i_index][j_index];
				}
			}

			connection[i_index][nearest] = 1;
			connection[nearest][i_index] = 1;

			int diff_x = room[nearest].x - room[i_index].x;
			int diff_y = room[nearest].y - room[i_index].y;

			queue_connection[queue_size++] = i_index;

			if (diff_x > 0)
			{
				if (diff_y > 0)
				{
					pos pA, pB;

					pA.x = room[i_index].x + (rand() % room_size[i_index].x_size);
					pA.y = room[i_index].y;

					pB.x = room[nearest].x;
					pB.y = room[nearest].y + (rand() % room_size[nearest].y_size);

					int dX = pB.x - pA.x;
					int dY = pB.y - pA.y;

					for (int j = pA.y; j != pB.y; (dY > 0) ? ++j : --j)
					{
						if (data[j][pA.x] == tile_none) setTileFloor(data, pA.x, j); // data[j][pA.x] = tile_floor;
					}

					for (int j = pA.x; j != pB.x; (dX > 0) ? ++j : --j)
					{
						if (data[pB.y][j] == tile_none) setTileFloor(data, j, pB.y); // data[pB.y][j] = tile_floor;
					}

				}
				else
				{
					pos pA, pB;

					pA.x = room[i_index].x + room_size[i_index].x_size - 1;
					pA.y = room[i_index].y + (rand() % room_size[i_index].y_size);

					pB.x = room[nearest].x + (rand() % room_size[nearest].x_size);
					pB.y = room[nearest].y;

					int dX = pB.x - pA.x;
					int dY = pB.y - pA.y;

					for (int j = pA.x; j != pB.x; (dX > 0) ? ++j : --j)
					{
						if (data[pA.y][j] == tile_none) setTileFloor(data, j, pA.y);
					}

					for (int j = pA.y; j != pB.y; (dY > 0) ? ++j : --j)
					{
						if (data[j][pB.x] == tile_none) setTileFloor(data, pB.x, j);
					}
				}
			}
			else
			{
				if (diff_y > 0)
				{
					pos pA, pB;

					pA.x = room[i_index].x;
					pA.y = room[i_index].y + (rand() % room_size[i_index].y_size);

					pB.x = room[nearest].x + (rand() % room_size[nearest].x_size);
					pB.y = room[nearest].y + room_size[nearest].y_size - 1;

					int dX = pB.x - pA.x;
					int dY = pB.y - pA.y;

					for (int j = pA.x; j != pB.x; (dX > 0) ? ++j : --j)
					{
						if (data[pA.y][j] == tile_none) setTileFloor(data, j, pA.y);
					}

					for (int j = pA.y; j != pB.y; (dY > 0) ? ++j : --j)
					{
						if (data[j][pB.x] == tile_none) setTileFloor(data, pB.x, j);
					}
				}
				else
				{
					pos pA, pB;

					pA.x = room[i_index].x + (rand() % room_size[i_index].x_size);
					pA.y = room[i_index].y + room_size[i_index].y_size - 1;

					pB.x = room[nearest].x + room_size[nearest].x_size - 1;
					pB.y = room[nearest].y + (rand() % room_size[nearest].y_size);

					int dX = pB.x - pA.x;
					int dY = pB.y - pA.y;

					for (int j = pA.y; j != pB.y; (dY > 0) ? ++j : --j)
					{
						if (data[j][pA.x] == tile_none) setTileFloor(data, pA.x, j);
					}

					for (int j = pA.x; j != pB.x; (dX > 0) ? ++j : --j)
					{
						if (data[pB.y][j] == tile_none) setTileFloor(data, j, pB.y);
					}
				}
			}

		}
	}

	for (int i = 0; i < y_size; ++i)
	{
		for (int j = 0; j < x_size; ++j)
		{
			if (data[i][j] == tile_none)
			{
				if (i > 0 && isWalkable(data,j,i-1))
				{
					data[i][j] = tile_wall;
				}
				if (i < y_size-1 && isWalkable(data,j, i + 1))
				{
					data[i][j] = tile_wall;
				}
				if (j > 0 && isWalkable(data, j - 1, i))
				{
					data[i][j] = tile_wall;
				}
				if (j < x_size - 1 && isWalkable(data, j+1, i))
				{
					data[i][j] = tile_wall;
				}
			}
		}
	}
}
