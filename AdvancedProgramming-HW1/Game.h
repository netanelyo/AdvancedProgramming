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
	Game(std::string movesFileA, std::string movesFileB) : nextPlayer(0), A(movesFileA), B(movesFileB) {}
	~Game() {}
	int checkAndCreateBoard(std::ifstream & BoardFile); 
	GameState playMove(); 
	void createBoardsForPlayers(); 

private:
	int nextPlayer; 
	Player A;
	Player B; 
	char gameBoard[BOARD_SIZE][BOARD_SIZE]; 
	 
	void createBoardForPlayer(int player, char** board); 
	

};