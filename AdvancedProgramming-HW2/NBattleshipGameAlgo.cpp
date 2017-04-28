#include "NBattleshipGameAlgo.h"


std::pair<int, int> NBattleshipGameAlgo::attack()
{
	auto attackPair = m_myBoard.getNextEmptySquare(m_currentMove);
	
	m_currentMove.first		= attackPair.first++;
	m_currentMove.second	= attackPair.second++;

	return attackPair;
}

