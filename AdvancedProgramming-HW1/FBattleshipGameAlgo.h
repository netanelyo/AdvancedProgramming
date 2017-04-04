#pragma once

#include "IBattleshipGameAlgo.h"
#include <fstream>

class FBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	
private:
	char** board;
	int points;
	std::ifstream moves;
};



