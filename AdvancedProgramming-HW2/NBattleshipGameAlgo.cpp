#include "NBattleshipGameAlgo.h"

void NBattleshipGameAlgo::setBoard(int player, const char** board, int numRows, int numCols)
{
	
}

std::pair<int, int> NBattleshipGameAlgo::attack()
{
	auto tmp = m_currentMove.second;
	auto cond = tmp < BattleshipConstants::BOARD_SIZE - 1;
	auto col = cond ? (tmp + 1) : 0;

	tmp = m_currentMove.first;
	auto row = cond ? tmp : tmp + 1;
	
	std::pair<int, int> retPair;

	if (m_currentMove.first == -1 && m_currentMove.second == -1)
	{
		retPair.first	= 1;
		retPair.second	= 1;
	}
	else
	{
		while (m_board[row][col] != '0')
		{
			if (col == BattleshipConstants::BOARD_SIZE - 1)
			{
				col = 0;
				row++;
			}
			else
				col++;
		}

		retPair.first	= row + 1;
		retPair.second	= col + 1;
	}

	return retPair;
}

bool NBattleshipGameAlgo::getNextValidSqaure(int i, int j) const
{
	return false;
}

