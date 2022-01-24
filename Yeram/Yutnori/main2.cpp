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
private:
	Space* head;
public:
	bool isActive;
	char shape;
	Space* space;
	Token(char shape, Space* head) :shape(shape), space(head), head(head), isActive(true) {}
	void moveNext()
	{
		space = space->link;
		if (space->pos == make_pair(30, 30))
		{
			isActive = false;
			return;
		}
	}

	pair<int, int> getPos()
	{
		if (shape == 'A')
			return space->pos;
		else if (shape == 'B')
			return make_pair(space->pos.first + 1, space->pos.second);
		else if (shape == 'C')
			return make_pair(space->pos.first, space->pos.second + 1);
		else if (shape == 'D')
			return make_pair(space->pos.first + 1, space->pos.second + 1);
	}

	pair<int, int> getSpacePos()
	{
		return space->pos;
	}

	void setAvtive() {
		isActive = true;
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
	for (int i = 24; 0 <= i; i -= 6)
	{
		Space* node = new Space(head, temp, make_pair(i, 0));
		temp->link = node;
		temp = node;
	}

	//(X,0)
	for (int i = 6; i<= 30; i += 6)
	{
		Space* node = new Space(head, temp, make_pair(0, i));
		temp->link = node;
		temp = node;
	}
	//(X,30)
	for (int i = 6; i <30; i += 6)
	{
		Space* node = new Space(head, temp, make_pair(i,30));
		temp->link = node;
		temp = node;
	}
}

void linkSpace(Token* token) //현재위치 노드를 삽입.
{
	Space* head = token->space;
	pair<int, int> pos = head->pos;
	Space* temp = head;//임시 포인터

	if (pos == make_pair(30, 0))
	{
		for (int i = 0; i <= 5; i++)
		{
			Space* node = new Space(head, temp, make_pair(5 * (5 - i), (i + 1) * 5));
			temp->link = node;
			temp = node;
		}

		for (int i = 6; i <= 30; i += 6)
		{
			Space* node = new Space(head, temp, make_pair(i, 30));
			temp->link = node;
			temp = node;
		}
	}
	else if (pos == make_pair(0, 0))
	{
		for (int i = 1; i <= 6; i++)
		{
			Space* node = new Space(head, temp, make_pair(5*i, 5 * i));
			temp->link = node;
			temp = node;
		}
	}
	else if (pos == make_pair(15, 15))
	{
		for (int i = 4; i <= 6; i++)
		{
			Space* node = new Space(head, temp, make_pair(5 * i, 5 * i));
			temp->link = node;
			temp = node;
		}
	}

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
	
	//setAvtive()
	for (auto token : player)
	{
		if (!token->isActive) continue;

		string tmp_string(1, token->shape);
		map[token->getPos().second].replace(token->getPos().first, 1, tmp_string);
	}
	

	//뒤에 \n 추가하기.
	vector<string>::iterator itor = map.begin();

	for (; itor != map.end(); itor++)
	{
		if(itor == map.end()-1)itor->push_back('\0');
		else itor->push_back('\n');
		char* cmap = new char[34];
		strcpy(cmap, itor->c_str());
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

		Token* target = player.front();
		for (auto token : player)
		{
			if (to_string(token->shape) != to_string(turn.front())) continue;
			target = token;
			break;
		}

		if (to_string(target->shape) != to_string(turn.front()))
		{
			target = new Token(turn.front(), head);
			player.push_back(target);
		}

		vector<Token*> group;
		for (auto member : player)
		{
			if (target->getSpacePos() == member->getSpacePos()) group.push_back(member);
		}


		while (step != 0)
		{
			for (auto member : group)
			{
				member->moveNext();
			}	
			step--;
		}

		//이동을 끝낸 지점이 모서리이면, 길을 바꾼다.
		for (auto member : group)
		{
			linkSpace(member);
		}

		count--;

		/*drawMap(head, player);
		printf("\n");*/
	}

	
	drawMap(head, player);
	return 0;
}