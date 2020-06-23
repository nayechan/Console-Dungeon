#include "GameUtility.h"
#include <time.h>

float deltaTime()
{
	static clock_t recent_time = -1;
	if (recent_time == -1) recent_time = clock();
	clock_t diff = clock() - recent_time;
	recent_time = clock();

	return (float)diff / (float)CLOCKS_PER_SEC;
}
