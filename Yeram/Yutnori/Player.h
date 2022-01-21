#pragma once
#include <vector>
#include "Token.h"
using namespace std;
class Player {
	vector<Token> tokens;
public:
	Player(Map* map, char nm1, char nm2, char nm3, char nm4)
	{
		tokens.push_back({ nm1,map });
		tokens.push_back({ nm2,map });
		tokens.push_back({ nm3,map });
		tokens.push_back({ nm4,map });
	}

	void moveToken(string turn);

	const vector<Token> getTokens() const { return tokens; }// read only
};



