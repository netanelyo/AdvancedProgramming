#include "FBattleshipGameAlgo.h"
#include <string>

void FBattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols)
{
	for (int i = 0; i < numRows; ++i)
		for (int j = 0; j < numCols; ++j)
			this->board[i][j] = board[i][j];
}

std::pair<int, int> FBattleshipGameAlgo::attack()
{
	std::string line("");
	//std::getline(playerMoves, )
	return std::pair<int, int>();
}

void FBattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	if (board[row][col] != '0')
	{
		switch (result)
		{
		case AttackResult::Sink:
			shipCounter--;
		case AttackResult::Hit:
			board[row][col] = 'X';
			break;
		default:
			break;
		}
	}
}
