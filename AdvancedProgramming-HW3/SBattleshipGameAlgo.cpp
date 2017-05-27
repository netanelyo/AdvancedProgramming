#include "SBattleshipGameAlgo.h"
#include <iostream> 

void SBattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	row--;
	col--;

	if (!m_myBoard.validateCoordinate(row, col))
		return;

	switch (result)
	{
	case AttackResult::Miss:
		m_myBoard.setBoardSquare(row, col, '#');
		break;

	case AttackResult::Hit:
		hitNotify(row, col, player);
		break;

	case AttackResult::Sink:
		sinkNotify(row, col, player);
		break;

	default:
		break;
	}
}

std::pair<int, int> SBattleshipGameAlgo::attack()
{
	/*if we are in sequential state (or have no more moves) we simply use our father's function*/
	if (isSequntial() || isDone())
	{
		return CommonBattleshipGameAlgo::attack();
	}
	/*finds the next attack on the ship we are currently trying to drown*/
	auto& nextAttackDirection = m_attackHelperMap[m_currentAttackingShip];
	int row = m_currentAttackingShip.first;
	int col = m_currentAttackingShip.second;
	/*if we have no direction, we first check our borders to see if we can get a direction*/
	if (nextAttackDirection == Direction::NON)
	{
		if ((col < m_myBoard.getBoardCols() - 1 && m_myBoard.getBoardSquare(row, col + 1) == 'X')
			|| (col > 0 && m_myBoard.getBoardSquare(row, col - 1) == 'X'))
		{
			nextAttackDirection = Direction::HORIZONTAL;
		}

		else if ((row < m_myBoard.getBoardRows() - 1 && m_myBoard.getBoardSquare(row + 1, col) == 'X')
			|| (row > 0 && m_myBoard.getBoardSquare(row - 1, col) == 'X'))
		{
			nextAttackDirection = Direction::VERTICAL;
		}
	}
	/*finds the next empty square in the ship's direction (or in its surrounding if it has direction NON)*/
	return findNextEmptySquare(row, col, nextAttackDirection);
}

void SBattleshipGameAlgo::markOppSankShip(int row, int col, bool& changed)
{
	auto sq = m_myBoard.getBoardSquare(row, col);
	m_myBoard.setBoardSquare(row, col, '#');
	if (sq != 'X')
	{
		return;
	}

	std::pair<int, int> tempCoor(row, col);
	if (m_preferredAttackSquares.find(tempCoor) != m_preferredAttackSquares.end())
	{
		if (m_currentAttackingShip == tempCoor)
			changed = true;

		m_preferredAttackSquares.erase(tempCoor);
		m_attackHelperMap.erase(tempCoor);
	}

	if (row > 0)
	{
		markRecursiveCall(row - 1, col, changed);
	}

	if (col > 0)
	{
		markRecursiveCall(row, col - 1, changed);
	}

	if (row < m_myBoard.getBoardRows() - 1)
	{
		markRecursiveCall(row + 1, col, changed);
	}

	if (col < m_myBoard.getBoardCols() - 1)
	{
		markRecursiveCall(row, col + 1, changed);
	}
}

void SBattleshipGameAlgo::markRecursiveCall(int row, int col, bool& changed)
{
	auto sq = m_myBoard.getBoardSquare(row, col);
	if (sq == 'X')
		markOppSankShip(row, col, changed);

	else if (sq == '0')
		m_myBoard.setBoardSquare(row, col, '#');
}

void SBattleshipGameAlgo::findNextMove()
{
	if (m_preferredAttackSquares.empty())
	{
		m_sequentialState = true;
	}
	else
	{
		auto iter = m_preferredAttackSquares.begin();
		m_currentAttackingShip = *iter;
	}
}

void SBattleshipGameAlgo::hitNotify(int row, int col, int player)
{
	auto sq = m_myBoard.getBoardSquare(row, col);
	m_myBoard.setBoardSquare(row, col, 'X');

	if (sq == '0') /* Opponent's ship*/
	{
		auto tmpPair = std::pair<int, int>(row, col);

		if (isSequntial() || player != getPlayerID())
		{
			if (isSequntial()) {
				m_currentAttackingShip = tmpPair;
				m_sequentialState = false;
			}

			m_attackHelperMap[tmpPair] = Direction::NON;
			m_preferredAttackSquares.insert(tmpPair);
		}

		else
			m_attackHelperMap[m_currentAttackingShip] = m_lastAttackDirection;
	}
}

void SBattleshipGameAlgo::sinkNotify(int row, int col, int player)
{
	if (m_myBoard.getBoardSquare(row, col) != '0')
	{
		CommonBattleshipGameAlgo::notifyOnAttackResult(player, row + 1, col + 1, AttackResult::Sink);
	}
	else
	{
		auto changed = false;
		m_myBoard.setBoardSquare(row, col, 'X');
		markOppSankShip(row, col, changed);
		if (changed)
		{
			findNextMove();
		}
	}
}

std::pair<int, int> SBattleshipGameAlgo::findNextEmptySquare(int row, int col, Direction nextAttackDirection)
{
	std::pair<int, int> retPair;

	if (col < m_myBoard.getBoardCols() - 1 && nextAttackDirection != Direction::VERTICAL &&
		m_myBoard.getBoardSquare(row, col + 1) == '0')
	{
		m_lastAttackDirection = Direction::HORIZONTAL;
		retPair = std::pair<int, int>(row + 1, col + 2);
	}

	else if (col > 0 && nextAttackDirection != Direction::VERTICAL &&
		m_myBoard.getBoardSquare(row, col - 1) == '0')
	{
		m_lastAttackDirection = Direction::HORIZONTAL;
		retPair = std::pair<int, int>(row + 1, col);
	}

	else if (row < m_myBoard.getBoardRows() - 1 && nextAttackDirection != Direction::HORIZONTAL &&
		m_myBoard.getBoardSquare(row + 1, col) == '0')
	{
		m_lastAttackDirection = Direction::VERTICAL;
		retPair = std::pair<int, int>(row + 2, col + 1);
	}

	else if (row > 0 && nextAttackDirection != Direction::HORIZONTAL &&
		m_myBoard.getBoardSquare(row - 1, col) == '0')
	{
		m_lastAttackDirection = Direction::VERTICAL;
		retPair = std::pair<int, int>(row, col + 1);
	}

	else
	{
		retPair = findNextIterative(row, col, nextAttackDirection);
	}

	return retPair;
}

std::pair<int, int> SBattleshipGameAlgo::findNextIterative(int row, int col, Direction nextAttackDirection) const
{
	switch (nextAttackDirection)
	{
	case Direction::VERTICAL:
	{
		auto tmpRow = row;
		char sq;
		while (tmpRow < m_myBoard.getBoardRows() - 1 &&
			(sq = m_myBoard.getBoardSquare(tmpRow + 1, col)) != '#')
		{
			if (sq == 'X')
				++tmpRow;

			else if (sq == '0')
				return std::pair<int, int>(tmpRow + 2, col + 1);
		}

		tmpRow = row;
		while (tmpRow > 0 && (sq = m_myBoard.getBoardSquare(tmpRow - 1, col)) != '#')
		{
			if (sq == 'X')
				--tmpRow;

			else if (sq == '0')
				return std::pair<int, int>(tmpRow, col + 1);
		}
		break;
	}

	case Direction::HORIZONTAL:
	{
		auto tmpCol = col;
		char sq;
		while (tmpCol < m_myBoard.getBoardCols() - 1 &&
			(sq = m_myBoard.getBoardSquare(row, tmpCol + 1)) != '#')
		{
			if (sq == 'X')
				++tmpCol;

			else if (sq == '0')
				return std::pair<int, int>(row + 1, tmpCol + 2);
		}

		tmpCol = col;
		while (tmpCol > 0 && (sq = m_myBoard.getBoardSquare(row, tmpCol - 1)) != '#')
		{
			if (sq == 'X')
				--tmpCol;

			else if (sq == '0')
				return std::pair<int, int>(row + 1, tmpCol);
		}
		break;
	}

	default:
		break;
	}
	
	return std::pair<int, int>(-1, -1);
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new SBattleshipGameAlgo();
}
