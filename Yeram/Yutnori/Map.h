#pragma once
#include <string>
#include <cstring>
#include "Space.h"
#include "Linker.h"


class Map
{
private:
	char* map;
	Linker* linker;


	void setSpaceShape(int offset); //offset = array index

	void setLineShape();

	void setMapShape();

	friend struct Token;
public:
	Map() :map(new char[SIZE + 1]), linker(new Linker)
	{
		linker->linkMapData();
		setMapShape();
	}

	~Map() { delete map; delete linker; }

	char* getMap() const { return map; }

	void setMap(const int offset, const char shape) { map[offset] = shape; }

	const void drawMap() const { printf("%s", map); }
};

