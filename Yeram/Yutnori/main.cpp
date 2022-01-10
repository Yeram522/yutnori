#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define WIDTH 32
#define HEIGHT 32
#define SIZE 33*32

enum class Direction {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

//part of Map -> map의 구성요소이다.
typedef struct Space
{
	bool isEdge;//모서리인지 아닌지.
	int offset;//map상에서의 offset(position)
	//vector<Space*> link; //다음 Space 가르키는 주소<
	Space* link; //다음 Space 가르키는 주소

	Space(Space* head, Space* prevNode , int offset, bool isEdge = false)
	{
		//headnode,prevnode NULL 체크
		if (head == NULL || prevNode == NULL) exit(1);

		this->isEdge = isEdge;
		this->offset = offset;

		link = head;//원형리스트
	}

	void movePiece(char* map, char shape)
	{
		map[offset] = shape;
	}
}Space;

typedef struct Linker
{
private:
	Space* head;
public:
	void linkPrevNode(Space* node, Space* prevNode)
	{
		prevNode->link = node;
	}
}Linker;
typedef struct Map
{
private:
	char* map;
	Space* head;
	Linker* linker;
	//대각선 WIDTH * 6;
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

	void setSpaceShape(int offset) //offset = array index
	{
		int secondline = WIDTH + 1;
		if (0 <= offset && offset <= WIDTH) secondline = WIDTH + 2;
		map[offset] = '.';
		map[offset + 1] = '.';
		map[offset + secondline] = '.';
		map[offset + secondline + 1] = '.';
	}

	//draw line of map
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

	//linking Space node
	Space* linkSpace(Space* head, Space* startNode, int endOffset, Direction dir) {
		Space* temp = startNode; //임시 pointer
		int startOffset = startNode->offset + nextOffset(dir);

		if (dir == Direction::DOWN || dir == Direction::RIGHT) return linkSpace(head , temp, startOffset, endOffset, dir);

		for (int i = startOffset; endOffset <= i; i += nextOffset(dir))
		{
			Space* node = new Space(head, temp, i);
			linker->linkPrevNode(node, temp);
			setSpaceShape(i);
			temp = node;
		}

		return temp; //return lastNode
	}

	Space* linkSpace(Space* head, Space* temp, int startOffset,int endOffset, Direction dir)
	{
		//((i==0)?nextOffset(dir): nextOffset(dir)- 1)
		for (int i = startOffset; i < endOffset; i += nextOffset(dir))
		{
			Space* node = new Space(head, temp, i);
			linker->linkPrevNode(node, temp);
			setSpaceShape(i);
			temp = node;
			if (i != 0 && dir == Direction::DOWN) i--;
		}

		return temp; //return lastNode
	}
public:
	Map():map(new char[SIZE + 1]),head(new Space(head, head, (WIDTH + 1)* (HEIGHT - 1) - 2)),linker(new Linker)
	{
		memset(map, ' ', SIZE);
		map[SIZE] = '\0';
		setLineShape();
		linker->linkPrevNode(head, head);
	}

	//함수 내부에서 space들을 link 시킨다.
	void createMap()
	{
		//edge(0,0) = startPoint
		setSpaceShape((WIDTH + 1) * (HEIGHT - 1) - 2);
		//rightSide
		Space* lastSpace = linkSpace(head, head, WIDTH,Direction::UP);
	
		//edge(0,1)
		Space* Edge = new Space(head, lastSpace, lastSpace->offset + nextOffset(Direction::UP) - 1);
		linker->linkPrevNode(Edge, lastSpace);
		setSpaceShape(lastSpace->offset + nextOffset(Direction::UP) - 1);
		//upSide
		lastSpace = linkSpace(head, Edge, 1, Direction::LEFT);

		//edge(-1,1)
		Edge = new Space(head, lastSpace, lastSpace->offset + nextOffset(Direction::LEFT));
		linker->linkPrevNode(Edge, lastSpace);
		setSpaceShape(lastSpace->offset + nextOffset(Direction::LEFT));
		//leftSide
		lastSpace = linkSpace(head, Edge, SIZE + nextOffset(Direction::UP), Direction::DOWN);

		//edge(-1,0)
		Edge = new Space(head, lastSpace, lastSpace->offset + nextOffset(Direction::DOWN) - 1);
		linker->linkPrevNode(Edge, lastSpace);
		setSpaceShape(lastSpace->offset + nextOffset(Direction::DOWN) - 1);
		//downSide
		lastSpace = linkSpace(head, Edge, head->offset, Direction::RIGHT);

		return;
	}

	char* getMap() { return map; }

	//얘는 말 구조체가 수행해야 할 함수 인 것 같다.
	void moveNext(int count, char shape)
	{
		Space* tmp=head;
		while (1)
		{
			if (count == 0)
			{
				tmp->movePiece(map, shape);
				break;
			}
			
			tmp = tmp->link;		
			count--;
		}
	}
};

int main()
{
	int count;
	string turn;
	Map map = Map();
	int movedsteps[4] = {0,};//ABCD
	map.createMap();

	//횟수
	scanf("%d ", &count);

	while (count != 0)
	{
		getline(cin, turn);
		int index = turn.front() - 'A';
		for (auto data : turn)
		{
			if (data == 'F') movedsteps[index]++;
		}
		count--;
	}

	for (auto player : movedsteps)
	{
		static int i = 0; //static 써도 괜찮은강..? 아님 그냥 for문 index로 바꿔버릴까 고민중
		if (player == 0) continue;
		if(player <= 20) map.moveNext(player, 'A' + i);	 //20->한바퀴 의미
		i++;
	}
	
	printf("%s", map.getMap());
}