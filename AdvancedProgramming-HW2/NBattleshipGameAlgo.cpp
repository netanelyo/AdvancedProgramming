#include "NBattleshipGameAlgo.h"


std::pair<int, int> NBattleshipGameAlgo::attack()
{
	auto attackPair = m_myBoard.getNextEmptySquare(m_currentMove);
	
	attackPair.first++;
	attackPair.second++;

	return attackPair;
}

