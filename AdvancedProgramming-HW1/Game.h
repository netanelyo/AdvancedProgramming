#pragma once
#include "FBattleshipGameAlgo.h"

using Player = FBattleshipGameAlgo;

enum class GameState 
{
	GAME_OVER, 
	CONTINUE_PLAYING
};

class Game
{
public:
	Game(int rows, int columns, std::string movesFileA, std::string movesFileB);
	~Game(); 
	int checkAndCreateBoard(std::ifstream & BoardFile); 
	GameState playMove(); 
	void createBoardsForPlayers(); 

private:
	int nextPlayer; 
	Player A;
	Player B; 
	int rowsNum;
	int colsNum;
	char** gameBoard; 
	 
	

};