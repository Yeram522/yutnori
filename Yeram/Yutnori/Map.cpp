#include "Map.h"

void Map::setSpaceShape(int offset)
{
	int secondline = WIDTH + 1;
	if (0 <= offset && offset <= WIDTH) secondline = WIDTH + 2;
	map[offset] = '.';
	map[offset + 1] = '.';
	map[offset + secondline] = '.';
	map[offset + secondline + 1] = '.';
}

void Map::setLineShape()
{
	//DrawLine
	int diagcount = 0;//diagonal count
	for (int i = 0; i < SIZE; i++)
	{
		if ((0 <= i && i < WIDTH) || ((WIDTH + 1) * (HEIGHT - 1) - 1 <= i && i < SIZE)) map[i] = '-';
		if (WIDTH < i && i % (WIDTH + 1) == 0)
		{
			if (i != WIDTH + 1) map[i - 1] = '|';
			map[i] = '\n';
			map[i + 1] = '|';

			//대각선
			if (diagcount != HEIGHT)
			{
				diagcount++;
				map[(i + 1) + diagcount] = '\\';
				map[(i + WIDTH) - diagcount] = '/';
			}
		}
	}
}

void Map::setMapShape()
{
	memset(map, ' ', SIZE);
	map[SIZE] = '\0';

	setLineShape();

	Space* temp = linker->getHead();
	do
	{
		setSpaceShape(temp->getOffset());
		temp = temp->link;
	} while (temp != linker->getHead());

	//대각선 그리기;
	int count = 5;
	while (count != -1)
	{
		setSpaceShape((WIDTH * 5) * (count + 1) + WIDTH - 1);
		setSpaceShape(((WIDTH + 2) * 5) * (count + 1) + 1);
		count--;
	}
}