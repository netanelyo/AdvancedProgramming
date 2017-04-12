#pragma once
#include "FBattleshipGameAlgo.h"

#define RUBBER_BOAT_POINTS 2
#define MISSILE_BOAT_POINTS 3
#define SUBMARINE_POINTS 7
#define DESTROYER_POINTS 8
#define RUBBER_BOAT_LEN 1
#define MISSILE_BOAT_LEN 2
#define SUBMARINE_LEN 3
#define DESTROYER_LEN 4

using Player = FBattleshipGameAlgo;

enum class GameState 
{
	GAME_OVER, 
	CONTINUE_PLAYING
};

class Game
{
public:
	Game(std::string movesFileA, std::string movesFileB) : nextPlayer(0), A(movesFileA), B(movesFileB) {}
	~Game() {}
	int checkAndCreateBoard(std::ifstream & boardFile); 
	GameState playMove(); 
	void createBoardsForPlayers(); 

private:
	int nextPlayer; 
	Player A;
	Player B; 
	char gameBoard[BOARD_SIZE][BOARD_SIZE]; 

	void printEndOfGame(int winner); 
	bool endOfAttacks(); 
	AttackResult determineAttackResult(char square, int xCoord, int yCoord);
	void handlePointsAndNextTurn(AttackResult result, char ship, int currentPlayer, bool isAship);
	void dfsShip(char currShip, int dummy[][BOARD_SIZE], int row,
			int col, int* shipLen, int right, int* invalidShape, int* adjShips);
	
	

};