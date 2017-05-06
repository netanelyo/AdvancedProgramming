#include "SBattleshipGameAlgo.h"

/* Reminder: intPair = std::pair<int, int> */

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

intPair SBattleshipGameAlgo::attack()
{
	if (isSequntial() || isDone())
	{
		return CommonBattleshipGameAlgo::attack();
	}

	auto& nextAttackDirection = m_attackHelperMap[m_currentAttackingShip];
	int row = m_currentAttackingShip.first;
	int col = m_currentAttackingShip.second;

	if (nextAttackDirection == Direction::NON)
	{
		if ((col < m_myBoard.getBoardCols() - 1 && m_myBoard.getBoardSquare(row, col + 1) == 'X')
			|| (col > 0 && m_myBoard.getBoardSquare(row, col - 1) == 'X'))
		{
			m_lastAttackDirection = Direction::HORIZONTAL;
		}

		else if ((row < m_myBoard.getBoardRows() - 1 && m_myBoard.getBoardSquare(row + 1, col) == '0')
			|| (row > 0 && m_myBoard.getBoardSquare(row - 1, col) == '0'))
		{
			m_lastAttackDirection = Direction::VERTICAL;
		}
	}

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

	intPair tempCoor(row, col);
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
		auto tmpPair = intPair(row, col);

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

intPair SBattleshipGameAlgo::findNextEmptySquare(int row, int col, Direction nextAttackDirection)
{
	intPair retPair(-1, -1);

	if (col < m_myBoard.getBoardCols() - 1 && nextAttackDirection != Direction::VERTICAL &&
		m_myBoard.getBoardSquare(row, col + 1) == '0')
	{
		m_lastAttackDirection = Direction::HORIZONTAL;
		retPair = intPair(row + 1, col + 2);
	}

	else if (col > 0 && nextAttackDirection != Direction::VERTICAL &&
		m_myBoard.getBoardSquare(row, col - 1) == '0')
	{
		m_lastAttackDirection = Direction::HORIZONTAL;
		retPair = intPair(row + 1, col);
	}

	else if (row < m_myBoard.getBoardRows() - 1 && nextAttackDirection != Direction::HORIZONTAL &&
		m_myBoard.getBoardSquare(row + 1, col) == '0')
	{
		m_lastAttackDirection = Direction::VERTICAL;
		retPair = intPair(row + 2, col + 1);
	}

	else if (row > 0 && nextAttackDirection != Direction::HORIZONTAL &&
		m_myBoard.getBoardSquare(row - 1, col) == '0')
	{
		m_lastAttackDirection = Direction::VERTICAL;
		retPair = intPair(row, col + 1);
	}

	else
	{
		retPair = findNextIterative(row, col, nextAttackDirection);
	}

	return retPair;
}

intPair SBattleshipGameAlgo::findNextIterative(int row, int col, Direction nextAttackDirection) const
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
				return intPair(tmpRow + 2, col + 1);
		}

		tmpRow = row;
		while (tmpRow > 0 && (sq = m_myBoard.getBoardSquare(tmpRow - 1, col)) != '#')
		{
			if (sq == 'X')
				--tmpRow;

			else if (sq == '0')
				return intPair(tmpRow, col + 1);
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
				return intPair(row + 1, tmpCol + 2);
		}

		tmpCol = col;
		while (tmpCol > 0 && (sq = m_myBoard.getBoardSquare(row, tmpCol + 1)) != '#')
		{
			if (sq == 'X')
				--tmpCol;

			else if (sq == '0')
				return intPair(row + 1, tmpCol);
		}
		break;
	}

	default:
		break;
	}
	return intPair();
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new SBattleshipGameAlgo();
}
