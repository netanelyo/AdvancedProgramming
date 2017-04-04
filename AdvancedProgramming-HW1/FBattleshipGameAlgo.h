#pragma once

#include "IBattleshipGameAlgo.h"
#include <fstream>

class FBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	FBattleshipGameAlgo(const std::string movesFile) : board(nullptr), numRows(0), numCols(0), points(0),
							shipCounter(0), playerMoves(movesFile) {}
	~FBattleshipGameAlgo();
	void setBoard(const char** board, int numRows, int numCols) override;
	std::pair<int, int> attack() override;													
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
private:
	char** board;
	uint16_t numRows;
	uint16_t numCols;
	uint16_t points;
	uint16_t shipCounter;
	std::ifstream playerMoves;
};



