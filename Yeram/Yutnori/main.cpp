#include <iostream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;

#define WIDTH 32
#define HEIGHT 32
#define SIZE 33*32

struct Space
{
private:
	bool isEdge;//모서리인지 아닌지.
	int offset;//map상에서의 offset(position)
	//vector<Space*> link; //다음 Space 가르키는 주소<
public:
	Space* link; //다음 Space 가르키는 주소

	Space(Space* head, Space* prevNode , int offset, bool isEdge = false)
	{
		//headnode,prevnode NULL 체크
		if (head == NULL || prevNode == NULL) exit(0);

		this->isEdge = isEdge;
		this->offset = offset;
		link = head;//원형리스트
	}

	const int getOffset() const { return offset; };
};

struct Linker
{
private:
	Space* head;

	enum class Direction {
		UP,
		DOWN,
		RIGHT,
		LEFT
	};
	
	int nextOffset(Direction dir) {
		switch (dir)
		{
		case Direction::UP:
			return -(WIDTH + 1) * 6;
		case Direction::DOWN:
			return (WIDTH + 1) * 6 + 1;
		case Direction::RIGHT:
			return 6;
		case Direction::LEFT:
			return -6;
		default:
			break;
		}
	}

	//linking Space node
	Space* linkSpace(Space* startNode, int endOffset, Direction dir){
		Space* temp = startNode; //임시 pointer
		int startOffset = startNode->getOffset() + nextOffset(dir);

		if (dir == Direction::DOWN || dir == Direction::RIGHT) return linkSpace(temp, startOffset, endOffset, dir);

		for (int i = startOffset; endOffset <= i; i += nextOffset(dir))
		{
			Space* node = new Space(head, temp, i);
			temp->link = node;
			temp = node;
		}

		return temp; //return lastNode
	}

	Space* linkSpace(Space* temp, int startOffset, int endOffset, Direction dir) 
	{
		//((i==0)?nextOffset(dir): nextOffset(dir)- 1)
		for (int i = startOffset; i < endOffset; i += nextOffset(dir))
		{
			Space* node = new Space(head, temp, i);
			temp->link = node;
			temp = node;
			if (i != 0 && dir == Direction::DOWN) i--;
		}

		return temp; //return lastNode
	}
public:
	Linker() :head(new Space(head, head, (WIDTH + 1)* (HEIGHT - 1) - 2))
	{}

	~Linker() { delete head; }

	Space* getHead()  const  { return head; }

	void linkMapData()
	{
		//edge(0,0) = startPoint
		head->link = head;

		//rightSide
		Space* lastSpace = linkSpace(head, WIDTH, Direction::UP);

		//edge(0,1)
		Space* Edge = new Space(head, lastSpace, lastSpace->getOffset() + nextOffset(Direction::UP) - 1);
		lastSpace->link = Edge;

		//upSide
		lastSpace = linkSpace(Edge, 1, Direction::LEFT);

		//edge(-1,1)
		Edge = new Space(head, lastSpace, lastSpace->getOffset() + nextOffset(Direction::LEFT));
		lastSpace->link = Edge;
		//leftSide
		lastSpace = linkSpace(Edge, SIZE + nextOffset(Direction::UP), Direction::DOWN);

		//edge(-1,0)
		Edge = new Space(head, lastSpace, lastSpace->getOffset() + nextOffset(Direction::DOWN) - 1);
		lastSpace->link = Edge;
		//downSide
		lastSpace = linkSpace(Edge, head->getOffset(), Direction::RIGHT);

		return;
	}
};

struct Map
{
private:
	char* map;
	Linker* linker;

	
	void setSpaceShape(int offset) //offset = array index
	{
		int secondline = WIDTH + 1;
		if (0 <= offset && offset <= WIDTH) secondline = WIDTH + 2;
		map[offset] = '.';
		map[offset + 1] = '.';
		map[offset + secondline] = '.';
		map[offset + secondline + 1] = '.';
	}

	void setLineShape()
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

	void setMapShape()
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

	friend struct Token;
public:
	Map():map(new char[SIZE + 1]),linker(new Linker)
	{
		linker->linkMapData();
		setMapShape();	

		/*vector<string> mapsdata =
		{
			"..----..----..----..----..----..\n",
			"..    ..    ..    ..    ..    ..\n",
			"| \                          / |\n",
			"|  \                        /  |\n",
			"|   \                      /   |\n",
			"|    ..                  ..    |\n",
			"..   ..                  ..   ..\n",
			"..     \                /     ..\n",
			"|       \              /       |\n",
			"|        \            /        |\n",
			"|         ..        ..         |\n",
			"|         ..        ..         |\n",
			"..          \      /          ..\n",
			"..           \    /           ..\n",
			"|             \  /             |\n",
			"|              ..              |\n",
			"|              ..              |\n",
			"|             /  \             |\n",
			"..           /    \           ..\n",
			"..          /      \          ..\n",
			"|         ..        ..         |\n",
			"|         ..        ..         |\n",
			"|        /            \        |\n",
			"|       /              \       |\n",
			"..     /                \     ..\n"
			"..   ..                  ..   ..\n",
			"|    ..                  ..    |\n",
			"|   /                      \   |\n",
			"|  /                        \  |\n",
			"| /                          \ |\n",
			"..    ..    ..    ..    ..    ..\n",
			"..----..----..----..----..----..\n"
		};

		string maps;
		for (auto line : mapsdata)
		{
			maps.append(line);
		}
		
		strcpy(map, maps.c_str());*/
	}

	~Map() { delete map; delete linker; }

	char* getMap() const { return map; }

	void setMap(const int offset, const char shape) { map[offset] = shape; }

	const void drawMap() const{ printf("%s", map);}
};

struct Token {
	char shape; //말 모양
	bool active;
	bool exit;
	int localOffset;//말을 업게 될때 예외처리
	Space* currentSpace;
	Space* startSpace;
	int setLocalOffset(int index)
	{
		switch (index)
		{
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return (WIDTH + 1);
		case 3:
			return (WIDTH + 1) + 1;
		}
	}

public:
	Token(char shape, Map* map):localOffset(0), shape(shape), active(false), exit(false)
	{
		startSpace = map->linker->getHead();
		currentSpace = startSpace;
		localOffset = setLocalOffset(shape - 'A');
	}

	void moveNext()//다음노드로 이동.
	{
		if (active && currentSpace == startSpace) exit = true;
		if (!active) active = true;
		currentSpace = currentSpace->link;
	}

	const bool isActive() { return active; }
	const bool isExit() { return exit; }
	const char getShape() const { return shape; }
	const int getOffset() const { 
		if(0 <= currentSpace->getOffset() && currentSpace->getOffset() <= WIDTH)return currentSpace->getOffset() + localOffset + 1;
		return currentSpace->getOffset() + localOffset; 
	}
};

struct Player {
	vector<Token> tokens;
public:
	Player(Map* map,char nm1, char nm2, char nm3, char nm4)
	{
		tokens.push_back({ nm1,map });
		tokens.push_back({ nm2,map });
		tokens.push_back({ nm3,map });
		tokens.push_back({ nm4,map });
	}

	void moveToken(string turn)
	{
		int index = turn.front() - tokens.front().getShape();//token의 이름
		int step=0;
		for (auto data : turn)//B,F 개수 확인
		{
			if (data == 'F') step++; 
		}

		if (step == 0)step = 5;
		while (step != 0)
		{
			tokens[index].moveNext();
			step--;
		}
	}

	const vector<Token> getTokens() const { return tokens; }// read only
};

int main()
{
	int count;
	string turn;

	Map* map = new Map;
	Player* player = new Player(map,'A','B','C','D');

	scanf("%d ", &count);
	 
	while (count != 0)
	{
		getline(cin, turn);

		player->moveToken(turn);

		count--;
	}

	for (auto token : player->getTokens())
	{
		if (!token.isActive() || token.isExit()) continue;
		
		map->setMap(token.getOffset(), token.getShape());
	}

	map->drawMap();

	delete map;
	delete player;

	return 0;
}