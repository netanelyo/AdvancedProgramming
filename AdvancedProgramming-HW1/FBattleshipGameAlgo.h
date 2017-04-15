#pragma once

#include "IBattleshipGameAlgo.h"
#include "BattleshipUtils.h"
#include <fstream>
#include <string>
#include <cctype>

#define BOARD_SIZE 10
#define RUBBER_BOAT_POINTS 2
#define MISSILE_BOAT_POINTS 3
#define SUBMARINE_POINTS 7
#define DESTROYER_POINTS 8
#define RUBBER_BOAT_LEN 1
#define MISSILE_BOAT_LEN 2
#define SUBMARINE_LEN 3
#define DESTROYER_LEN 4

//TODO
#include <iostream>

class FBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	FBattleshipGameAlgo(const std::string movesFile) : points(0), shipCounter(0),
														playerMoves(movesFile), isDone(false) {}
	~FBattleshipGameAlgo() { playerMoves.close(); }
	void setBoard(const char** board, int numRows, int numCols) override;
	std::pair<int, int> attack() override;													
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	
	bool getIsDone() const { return isDone; }
	size_t getPoints() const { return points; }
	size_t getShipCounter() const { return shipCounter; }

	void setPoints(size_t pts) { points = pts; }
	void incrementShipCounter() { shipCounter++; }
	void setBoardCoord(int i, int j, char val) { board[i][j] = val; }

private:
	char			board[BOARD_SIZE][BOARD_SIZE];
	size_t			points;
	size_t			shipCounter;
	std::ifstream	playerMoves; 
	bool			isDone;
};



