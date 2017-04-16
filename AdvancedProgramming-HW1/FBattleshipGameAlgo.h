#pragma once

#include "IBattleshipGameAlgo.h"
#include "BattleshipUtils.h"
#include <fstream>
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

/**
 * class FBattleshipGameAlgo - implements IBattleshipGameAlgo by playing
 * a battleship game out of input files (board and moves)
 */
class FBattleshipGameAlgo : public IBattleshipGameAlgo
{
public:
	/**
	 * Constructor
	 * 
	 * @param movesFile - a moves filename to be opened as an ifstream
	 */
	FBattleshipGameAlgo(const std::string movesFile) : points(0), shipCounter(0),
														playerMoves(movesFile), isDone(false) {}

	/**
	 * Destructor - closes the moves file ifstream
	 */
	~FBattleshipGameAlgo() { playerMoves.close(); }

	/**
	 * @implements IBattleshipGameAlgo::setBoard pure virtual function
	 * 
	 * Copies board[i][j] to this->board[i][j]
	 * 
	 * @param board - the board to copy of
	 * @param numRows - board's number of rows
	 * @param numCols - board's number of columns
	 */
	void setBoard(const char** board, int numRows, int numCols) override;

	/**
	 * @implements IBattleshipGameAlgo::attack pure virtual function
	 * 
	 * reads from file the next attack move, if exists. 
	 * @return
	 * <-1, -1> in case of EOF;
	 * otherwise - a pair of positive integers as the next attack  
	 */
	std::pair<int, int> attack() override;													
	void notifyOnAttackResult(int player, int row, int col, AttackResult result) override;
	
	bool	getIsDone() const { return isDone; }
	size_t	getPoints() const { return points; }
	size_t	getShipCounter() const { return shipCounter; }

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



