#include "Game.h"


Game::Game(int rows, int columns, std::string movesFileA, std::string movesFileB) : 
	nextPlayer(0), A(movesFileA), B(movesFileB), rowsNum(rows), colsNum(columns)
{
	gameBoard = new char*[rows];

	for (int i = 0; i < rows; i++)
	{
		gameBoard[i] = new char[columns]; 
	}

}

Game::~Game()
{
	for (int i = 0; i < rowsNum; i++)
	{
		delete []gameBoard[i]; 
	}

	delete []gameBoard; 
}
