#include "FBattleshipGameAlgo.h"

void FBattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols)
{
	this->board = new char*[numRows];

	for (int i = 0; i < numRows; ++i) {
		this->board[i] = new char[numCols];
		for (int j = 0; j < numCols; ++j)
			this->board[i][j] = board[i][j];
	}
}

std::pair<int, int> FBattleshipGameAlgo::attack()
{

	return std::pair<int, int>();
}

void FBattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{

}
