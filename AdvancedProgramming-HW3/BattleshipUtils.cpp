#include "BattleshipUtils.h"


char ** BattleshipUtils::allocateBoard(int rows, int cols)
{
	char** board = new char*[rows];

	for (auto i = 0; i < rows; i++)
	{
		board[i] = new char[cols];
	}
	return board;
}

void BattleshipUtils::deallocateBoard(char ** board, int rows)
{
	for (auto i = 0; i < rows; i++)
	{
		delete[] board[i];
	}

	delete[] board;
}
