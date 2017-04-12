#include "FBattleshipGameAlgo.h"


void FBattleshipGameAlgo::setBoard(const char** board, int numRows, int numCols)
{
	for (int i = 0; i < numRows; ++i)
		for (int j = 0; j < numCols; ++j)
			this->board[i][j] = board[i][j];
}

std::pair<int, int> FBattleshipGameAlgo::attack()
{
	std::pair<int, int> attackCoordinate(-1, -1);
	std::string			line;
	
	/* Reads attacks file until a valid line or EOF have been reached */
	while (!std::getline(playerMoves, line).eof())
	{
		if (parseLineAndValidate(line, attackCoordinate))
			break;
	}

	std::cout << "Am I out????" << std::endl;

	/* If EOF reached then current player is done */
	if (playerMoves.eof())
	{
		isDone = true;
	}
	
	return attackCoordinate;
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


