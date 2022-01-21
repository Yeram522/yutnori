#pragma once
#include "Space.h"
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
	Space* linkSpace(Space* startNode, int endOffset, Direction dir) {
		Space* temp = startNode; //юс╫ц pointer
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

	Space* getHead()  const { return head; }

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