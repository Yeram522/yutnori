#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include "FrameWork.h"
#include "Player.h"
using namespace std;

class YutNoriFrameWork : public FrameWork
{
protected:
	virtual void BeginFrameWork();
	virtual void EndFrameWork();

protected:
	virtual void MainWork();

private:
	int count;
	string turn;
	Map* map;
	Player* player;
};

