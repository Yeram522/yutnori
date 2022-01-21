#include "Player.h"

void Player::moveToken(string turn)
{
	int index = turn.front() - tokens.front().getShape();//token의 이름
	int step = 0;
	for (auto data : turn)//B,F 개수 확인
	{
		if (data == 'F') step++;
	}

	if (step == 0) step = 5;
	while (step != 0)
	{
		tokens[index].moveNext();
		step--;
	}
}
