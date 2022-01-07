#include <iostream>
#include <vector>

#define WIDTH 32
#define HEIGHT 32
#define SIZE 33*32
using namespace std;


typedef struct Space
{
	bool isEdge = false;
	vector<Space*> link; //다음 Space 가르키는 주소

	void drawSpace()
	{

	}
}Space;

char* drawLine()
{
	char* map = new char[SIZE];
	memset(map, ' ', SIZE);

	int temp = 0;
	for (int i = 0; i < SIZE; i++)
	{
		if ((0 <= i && i < WIDTH) || ((WIDTH + 1) * (HEIGHT - 1) - 1 <= i && i < SIZE)) map[i] = '-';
		if (WIDTH < i && i % (WIDTH + 1) == 0)
		{
			if (i != WIDTH + 1) map[i - 1] = '|';
			map[i] = '\n';
			map[i + 1] = '|';

			//대각선
			if (temp != HEIGHT)
			{
				temp++;
				map[(i + 1) + temp] = '\\';
				map[(i + WIDTH) - temp] = '/';
			}
		}
	}
	map[SIZE] = '\0';

	return map;
}


int main()
{
	char* map;
	map = drawLine();
	printf("%s", map);
}