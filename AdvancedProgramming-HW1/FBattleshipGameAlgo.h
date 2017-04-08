#pragma once

#include "IBattleshipGameAlgo.h"
#include <fstream>

#define BOARD_SIZE 10

class FBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	FBattleshipGameAlgo(const std::string movesFile) : points(0),
							shipCounter(0), playerMoves(movesFile), isDone(false) {}
	~FBattleshipGameAlgo() {}
	void setBoard(const char** board, int numRows, int numCols) override;
	std::pair<int, int> attack() override;													
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	bool getIsDone() const {return isDone;}
private:
	char			board[BOARD_SIZE][BOARD_SIZE];
	uint16_t		points;
	uint16_t		shipCounter;
	std::ifstream	playerMoves;
	bool			isDone;
};



