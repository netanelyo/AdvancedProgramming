#pragma once
#include "FBattleshipGameAlgo.h"

using Player = FBattleshipGameAlgo;

enum class GameState 
{
	GAME_OVER, 
	CONTINUE_PLAYING
};

enum class Direction
{
	NON,
	DOWN,
	RIGHT,
};

class Game
{
public:
	Game(std::string movesFileA, std::string movesFileB): nextPlayer(0), A(movesFileA), B(movesFileB) {}
	~Game() {}
	bool checkAndCreateBoard(std::ifstream & boardFile);
	GameState playMove(); 
	void createBoardsForPlayers(); 

private:
	int		nextPlayer; 
	Player	A;
	Player	B; 
	char	gameBoard[BOARD_SIZE][BOARD_SIZE]; 

	void printEndOfGame(int winner); 
	bool endOfAttacks(); 
	AttackResult determineAttackResult(char square, int xCoord, int yCoord);
	void handlePointsAndNextTurn(AttackResult result, char ship, int currentPlayer,
																bool isAship, bool canPassTurn);
	void dfsShip(char currShip, int dummy[][BOARD_SIZE], int row,
			int col, int& shipLen, Direction direction, int& invalidShape, int& adjShips) const;
	void markAllOfSameShip(char currShip, int dummy[][BOARD_SIZE], int row,	int col,
					int& invalidShape, int& adjShips) const;
	bool printErrors(int shouldPrint[], std::string errors[]);
	void removeSankShip(int xCoord, int yCoord);
	size_t getShipLen(char ship);
	
	void printBoard(); //TODO remove
	

};