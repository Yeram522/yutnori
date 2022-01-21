#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cstring>
using namespace std;

struct Space
{
	Space* link;
	pair<int, int> pos;

	Space(Space* head, Space* prevNode, pair<int, int> pos)
	{
		this->pos = pos;
		link = head;//원형리스트
	}
};

struct Token
{
	char shape;
	Space* space;
	Token(char shape, Space* head):shape(shape), space(head) {}
	void moveNext()
	{
		space = space->link;
	}

	pair<int, int> getPos()
	{
		if (shape == 'A')
			return space->pos;
		else if (shape == 'B')
			return make_pair(space->pos.first ,space->pos.second + 1);
		else if (shape == 'C')
			return make_pair(space->pos.first+1, space->pos.second);
		else if (shape == 'D')
			return make_pair(space->pos.first+1, space->pos.second+1);
	}

};

void linkSpace(Space* head)
{
	Space* temp = head;//임시 포인터
	//(30,Y) ↑
	for (int i = 24; 0 <= i; i-=6)
	{
		Space* node = new Space(head, temp,make_pair(30, i));
		temp->link = node;
		temp = node;
	}
	//(0,Y)
	//(X,0)
	//(X,30)
	//(X != Y)
	//(X == Y)
}

void drawMap(Space* head, vector<Token*> player)
{	
	vector<string> map =
	{
		"..----..----..----..----..----..",
		"..    ..    ..    ..    ..    ..",
		"| \\                          / |",
		"|  \\                        /  |",
		"|   \\                      /   |",
		"|    ..                  ..    |",
		"..   ..                  ..   ..",
		"..     \\                /     ..",
		"|       \\              /       |",
		"|        \\            /        |",
		"|         ..        ..         |",
		"|         ..        ..         |",
		"..          \\      /          ..",
		"..           \\    /           ..",
		"|             \\  /             |",
		"|              ..              |",
		"|              ..              |",
		"|             /  \\             |",
		"..           /    \\           ..",
		"..          /      \\          ..",
		"|         ..        ..         |",
		"|         ..        ..         |",
		"|        /            \\        |",
		"|       /              \\       |",
		"..     /                \\     ..",
		"..   ..                  ..   ..",
		"|    ..                  ..    |",
		"|   /                      \\   |",
		"|  /                        \\  |",
		"| /                          \\ |",
		"..    ..    ..    ..    ..    ..",
		"..----..----..----..----..----.."
	};

	Space* linker = head; //for Read
	
	string tmp_string(1, player.front()->shape);
	map[player.front()->getPos().second].replace(player.front()->getPos().first, 1, tmp_string);

	//뒤에 \n 추가하기.
	for (auto line : map)
	{
		line.push_back('\n');
		char* cmap = new char[34];
		strcpy(cmap, line.c_str());
		printf("%s", cmap);//출력
	}
}


int main()
{
	Space* head = nullptr;
	vector<Token*> player;

	head = new Space(head, head, make_pair(30, 30));
	linkSpace(head);
	int count;
	string turn;

	int step = 0;

	scanf("%d ", &count);

	while (count != 0)
	{
		getline(cin, turn);

		for (auto data : turn)//B,F 개수 확인
		{
			if (data == 'F') step++;
		}

		if (step == 0) step = 5;
		//가상의 공간에서 말을 생성 및 이동 시킨다.
		if (player.empty())//말이 하나도 없으면 생성한다.
		{
			player.push_back(new Token(turn.front(), head));
		}

		while (step != 0)
		{
			for (auto token : player)
			{
				token->moveNext();
			}
			step--;
		}

		count--;
	}

	drawMap(head, player);
	return 0;
}