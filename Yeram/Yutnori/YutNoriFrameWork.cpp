#include "YutNoriFrameWork.h"

void YutNoriFrameWork::BeginFrameWork()
{
    count = 0;
    map = new Map;
    player = new Player(map, 'A', 'B', 'C', 'D');
}

void YutNoriFrameWork::EndFrameWork()
{
    map->drawMap();

    delete map;
    delete player;
}

void YutNoriFrameWork::MainWork()
{

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
}
