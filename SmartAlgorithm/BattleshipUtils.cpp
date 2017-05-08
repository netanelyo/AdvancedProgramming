#include "BattleshipUtils.h"


char ** BattleshipUtils::allocateBoard(size_t rows, size_t cols)
{
	char** board = new char*[rows];

	for (auto i = 0; i < rows; i++)
	{
		board[i] = new char[cols];
	}
	return board;
}

void BattleshipUtils::deallocateBoard(char ** board, size_t rows)
{
	for (auto i = 0; i < rows; i++)
	{
		delete[] board[i];
	}

	delete[] board;
}
