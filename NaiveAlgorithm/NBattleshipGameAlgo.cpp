#include "NBattleshipGameAlgo.h"


std::pair<int, int> NBattleshipGameAlgo::attack()
{
	if (isDone())
	{
		return std::pair<int, int>(-1, -1);
	}

	auto attackPair = m_myBoard.getNextEmptySquare(m_currentSeqMove);
	auto& row = attackPair.first;
	auto& col = attackPair.second;

	if (row == -1 && col == -1)
	{
		m_isDone = true;
		return attackPair;
	}

	m_currentSeqMove.first		= row++;
	m_currentSeqMove.second	= col++;

	return attackPair;
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new NBattleshipGameAlgo();
}
