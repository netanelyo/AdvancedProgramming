#pragma once

#include "IBattleshipGameAlgo.h"
#include "BattleshipUtils.h"
#include "ShipOnBoard.h"
#include <fstream>
#include <string>
#include <cctype>

//TODO
#include <iostream>

#define BOARD_SIZE 10
#define RUBBER_BOAT_POINTS 2
#define MISSILE_BOAT_POINTS 3
#define SUBMARINE_POINTS 7
#define DESTROYER_POINTS 8
#define RUBBER_BOAT_LEN 1
#define MISSILE_BOAT_LEN 2
#define SUBMARINE_LEN 3
#define DESTROYER_LEN 4

class FBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	FBattleshipGameAlgo(const std::string movesFile) : myShips(new ShipOnBoard[5]), points(0),
		shipCounter(0), playerMoves(movesFile), isDone(false) {}
	~FBattleshipGameAlgo() { playerMoves.close(); delete[]myShips; }
	void setBoard(const char** board, int numRows, int numCols) override;
	std::pair<int, int> attack() override;													
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	
	bool getIsDone() const { return isDone; }
	uint16_t getPoints() const { return points; }
	uint16_t getShipCounter() const { return shipCounter; }

	void setPoints(uint16_t pts) { points = pts; }
	void incrementShipCounter() { shipCounter++; }

private:
	ShipOnBoard*	myShips;
	uint16_t		points;
	uint16_t		shipCounter;
	std::ifstream	playerMoves; 
	bool			isDone;
};



