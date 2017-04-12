#pragma once

#include "IBattleshipGameAlgo.h"
#include <fstream>
#include <string>
#include <cctype>

#define BOARD_SIZE 10

class FBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	FBattleshipGameAlgo(const std::string movesFile) : points(0),
							shipCounter(0), playerMoves(movesFile), isDone(false) {}
	~FBattleshipGameAlgo() { playerMoves.close(); }
	void setBoard(const char** board, int numRows, int numCols) override;
	std::pair<int, int> attack() override;													
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	
	bool getIsDone() const { return isDone; }
	uint16_t getPoints() const { return points; }
	uint16_t getShipCounter() const { return shipCounter; }

	void setPoints(uint16_t pts) { points = pts; }
	void incrementShipCounter() { shipCounter++; }

private:
	char			board[BOARD_SIZE][BOARD_SIZE];
	uint16_t		points;
	uint16_t		shipCounter;
	std::ifstream	playerMoves;
	bool			isDone;
};



