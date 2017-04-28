#include "NBattleshipGameAlgo.h"


std::pair<int, int> NBattleshipGameAlgo::attack()
{
	std::pair<int, int> attackPair = m_myBoard.getNextEmptySquare(m_currentMove); 
	
	attackPair.first++; 
	attackPair.second++; 

	return attackPair; 
}

