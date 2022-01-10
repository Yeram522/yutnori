#include <iostream>
#include <vector>
#include <string>
using namespace std;

#define WIDTH 32
#define HEIGHT 32
#define SIZE 33*32

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

	~Space() { delete link; }

	void movePiece(char* map, char shape)
	{
		map[offset] = shape;
	}

	int getOffset() const { return offset; };
}Space;

typedef struct Linker
{
private:
	Space* head;

	enum class Direction {
		UP,
		DOWN,
		RIGHT,
		LEFT
	};
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

	//linking Space node
	Space* linkSpace(Space* startNode, int endOffset, Direction dir) {
		Space* temp = startNode; //임시 pointer
		int startOffset = startNode->offset + nextOffset(dir);

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
		Space* Edge = new Space(head, lastSpace, lastSpace->offset + nextOffset(Direction::UP) - 1);
		lastSpace->link = Edge;
		//upSide
		lastSpace = linkSpace(Edge, 1, Direction::LEFT);

		//edge(-1,1)
		Edge = new Space(head, lastSpace, lastSpace->offset + nextOffset(Direction::LEFT));
		lastSpace->link = Edge;
		//leftSide
		lastSpace = linkSpace(Edge, SIZE + nextOffset(Direction::UP), Direction::DOWN);

		//edge(-1,0)
		Edge = new Space(head, lastSpace, lastSpace->offset + nextOffset(Direction::DOWN) - 1);
		lastSpace->link = Edge;
		//downSide
		lastSpace = linkSpace(Edge, head->offset, Direction::RIGHT);

		return;
	}
}Linker;

typedef struct Map
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
			setSpaceShape(temp->offset);
			temp = temp->link;
		} while (temp != linker->getHead());

	}

	friend struct Token;
public:
	Map():map(new char[SIZE + 1]),linker(new Linker)
	{
		linker->linkMapData();
		setMapShape();	
	}

	~Map() { delete map; delete linker; }

	char* getMap() const { return map; }

	//얘는 말 구조체가 수행해야 할 함수 인 것 같다.
	void moveNext(int count, char shape)
	{
		Space* tmp= linker->getHead();
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
}Map;

typedef struct Token {
	char shape; //말 모양
	
	int worldOffset;
	int localOffset;//말을 업게 될때 예외처리
public:
	int step; //이동칸수
	Token(char shape):step(0)
	{

	}
	void moveNext()
	{
		step++;
	}

	void moveNext(Map* map, int count)
	{
		Space* tmp = map->linker->getHead();//이래도 되는 걸까/...?ㅠ
		while (1)
		{
			if (count == 0)
			{
				worldOffset = tmp->getOffset();//월드 포지션 저장.
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
	vector<Token> tokens
	{
		{'A'},
		{'B'},
		{'C'},
		{'D'}
	};

	scanf("%d ", &count);

	while (count != 0)
	{
		getline(cin, turn);
		int index = turn.front() - 'A';
		for (auto data : turn)
		{
			if (data == 'F') tokens[index].moveNext();
		}
		count--;
	}

	for (auto player : tokens)
	{
		static int i = 0; 
		if (player.step == 0) continue;
		if (player.step <= 20) map.moveNext(player.step, 'A' + i);
		i++;
	}

	printf("%s", map.getMap());
}