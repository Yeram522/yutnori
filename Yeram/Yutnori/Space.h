#pragma once

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

	Space(Space* head, Space* prevNode, int offset, bool isEdge = false)
	{
		//headnode,prevnode NULL 체크
		if (head == NULL || prevNode == NULL) exit(0);

		this->isEdge = isEdge;
		this->offset = offset;
		link = head;//원형리스트
	}

	const int getOffset() const { return offset; };
};


