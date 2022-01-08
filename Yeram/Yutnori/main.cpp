#include <iostream>
#include <vector>
using namespace std;

#define WIDTH 32
#define HEIGHT 32
#define SIZE 33*32

typedef struct Token;
typedef struct Map;
typedef struct Space;

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

	void createSpace(Space* head, Space* prevNode, int offset, char* map,bool isEdge = false)
	{
		//headnode,prevnode NULL 체크
		if (head == NULL || prevNode == NULL) exit(1);

		this->isEdge = isEdge; 
		this->offset = offset;

		prevNode->link = this;
		link = head;//원형리스트

		drawSpace(offset, map);
	}

	void drawSpace(int offset, char* map) //offset = array index
	{
		int secondline = WIDTH + 1;
		if(0<= offset && offset<=WIDTH) secondline = WIDTH + 2;
		map[offset] = '.';
		map[offset+1] = '.';
		map[offset+ secondline] = '.';
		map[offset + secondline + 1] = '.';
	}

}Space;

typedef struct Token
{
	char shape;
	Space* space; //위치
public:
	Token(char shape) : shape(shape)
	{
		//space = map->gethead();
	}//모든 토큰은 map의 head에서 시작한다.

	int getOffset() { return space->offset; }
	const char getShape() { return shape; };
	void setSpace(Space* space) { this->space = space; }
	const Space* getSpace() { return space; }

}Token;

typedef struct Map
{
private:
	char* map;
	Space* head;
	vector<Token*> tokens;
	char tokenShape;
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

	//draw line of map
	void drawLine()
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
			Space* node = new Space;
			node->createSpace(head, temp, i, map);
			temp = node;
		}

		return temp; //return lastNode
	}

	Space* linkSpace(Space* head, Space* temp, int startOffset,int endOffset, Direction dir)
	{
		for (int i = startOffset; i < endOffset; i += nextOffset(dir))
		{
			Space* node = new Space;
			node->createSpace(head, temp, i, map);
			temp = node;
			if (i != 0 && dir == Direction::DOWN) i--;
		}

		return temp; //return lastNode
	}
public:	
	Map():map(new char[SIZE + 1]),head(new Space), tokenShape(65)
	{
		memset(map, ' ', SIZE);
		map[SIZE] = '\0';
		drawLine();
	}

	char* getMap() { return map; }
	Space* gethead() { return head; }

	//함수 내부에서 space들을 link 시킨다.
	void createMap()
	{
		//edge(0,0) = startPoint
		head->createSpace(head, head, (WIDTH + 1) * (HEIGHT - 1) - 2, map);

		//rightSide
		Space* lastSpace = linkSpace(head, head, WIDTH,Direction::UP);
		
		//edge(0,1)
		Space* Edge = new Space;
		Edge->createSpace(head, lastSpace, lastSpace->offset + nextOffset(Direction::UP) - 1, map);

		//upSide
		lastSpace = linkSpace(head, Edge, 1, Direction::LEFT);

		//edge(-1,1)
		Edge = new Space;
		Edge->createSpace(head, lastSpace, lastSpace->offset + nextOffset(Direction::LEFT), map);

		//leftSide
		lastSpace = linkSpace(head, Edge, SIZE + nextOffset(Direction::UP), Direction::DOWN);

		//edge(-1,0)
		Edge = new Space;
		Edge->createSpace(head, lastSpace, lastSpace->offset + nextOffset(Direction::DOWN) - 1, map);

		//downSide
		lastSpace = linkSpace(head, Edge, head->offset, Direction::RIGHT);

#if debug
		for (int i = 6; i < WIDTH - 6; i += 6)
		{
			//up
			Space* node = new Space;
			node->createSpace(head, temp, i, map);

			//down
			node = new Space;
			node->createSpace(head, temp, i + (WIDTH + 1) * (HEIGHT - 2) + 1, map);

			//left
			node = new Space;
			node->createSpace(head, temp, (WIDTH + 1) * i + 1, map);

			//right
			node = new Space;
			node->createSpace(head, temp, (WIDTH + 1) * (i + 1) - 2, map);
		}
#endif	
		return;
	}
	
	void updateToken(Token* token){ map[token->getOffset()] = token->getShape(); }

	Token* getToken(char shape)
	{
		if (tokens.size() == 0) tokens.push_back(new Token(tokenShape));

		for (auto token : tokens)
		{
			//해당 토큰이 map에 존재 하면
			if (token->getShape() == shape)
			{
				return token;
			}

		}

		//존재하지 않으면 새 토큰 생성
		Token* newtoken = new Token(tokenShape++);
		tokens.push_back(newtoken);
		return newtoken;
	}

	void moveToken(int step)
	{
		Token* token = getToken(tokenShape);
		const Space* prev = token->getSpace();
		while (1)
		{
			if (step == 0)
			{
				updateToken(token);
				//space = tmp;
				return;
			}

			//setSpace
			token->setSpace(token->getSpace()->link);
			step--;
		}
		
	}
	
}Map;


int main()
{
	int count;
	vector<Token*> tokens;
	Map* map = new Map();
	map->createMap();
	
	printf("20이하의 숫자 입력\n");
	scanf("%d", &count);

	map->moveToken(count);
	//map->moveToken('A',count, map);
	//printf("\n %s", map->getMap());
	//map->moveToken('A', 1,  map);
	

	printf("%s", map->getMap());
}