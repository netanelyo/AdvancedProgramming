#include "SBattleshipGameAlgo.h"

/* Reminder: intPair = std::pair<int, int> */

void SBattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	// TODO check validity of row/col
	row--;
	col--;

	if (!m_myBoard.validateCoordinate(row, col))
		return;

	switch (result)
	{
	case AttackResult::Miss:
		m_myBoard.setBoardSquare(row, col, '#');
		if (player == getPlayerID() && !isSequntial())
		{
			missSetUpForNextAttack();
		}
		break;

	case AttackResult::Hit:

		break;

	case AttackResult::Sink:
	{
			
		if (m_myBoard.getBoardSquare(row, col) != '0')
		{
			CommonBattleshipGameAlgo::notifyOnAttackResult(player, row + 1, col + 1, result);
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
		break;
	}

	default:
		break;
	}
}

intPair SBattleshipGameAlgo::attack()
{
	//if (/*isdone())
	//{
	//	return intpair(-1, -1);
	//}*/

	if (isSequntial())
	{
		return CommonBattleshipGameAlgo::attack();
	}

	const auto& nextAttackLookup = m_attackHelperMap[m_currentAttackingShip];
	const auto& direction = std::get<0>(nextAttackLookup);
	const auto& lookupCoor = std::get<1>(nextAttackLookup);
	auto row = lookupCoor.first;
	auto col = lookupCoor.second;

	intPair retPair(-1, -1);

	if (col < m_myBoard.getBoardCols() - 1 && direction != Direction::VERTICAL &&
				m_myBoard.getBoardSquare(row, col + 1) == '0')
	{
		m_lastAttackDirection = Direction::HORIZONTAL;
		retPair = intPair(row + 1, col + 2);
	}

	else if (col > 0 && direction != Direction::VERTICAL &&
		m_myBoard.getBoardSquare(row, col - 1) == '0')
	{
		m_lastAttackDirection = Direction::HORIZONTAL;
		retPair = intPair(row + 1, col);
	}

	else if (row < m_myBoard.getBoardRows() - 1 && direction != Direction::HORIZONTAL &&
		m_myBoard.getBoardSquare(row + 1, col) == '0')
	{
		m_lastAttackDirection = Direction::VERTICAL;
		retPair = intPair(row + 2, col + 1);
	}

	else if (row < 0 && direction != Direction::HORIZONTAL &&
		m_myBoard.getBoardSquare(row - 1, col) == '0')
	{
		m_lastAttackDirection = Direction::VERTICAL;
		retPair = intPair(row, col + 1);
	}

	return retPair;
}

void SBattleshipGameAlgo::markOppSankShip(int row, int col, bool& changed)
{
	m_myBoard.setBoardSquare(row, col, '#');
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

void SBattleshipGameAlgo::missSetUpForNextAttack()
{
	auto& nextAttackLookup = m_attackHelperMap[m_currentAttackingShip];
	auto& direction = std::get<0>(nextAttackLookup);
	auto& lookupCoor = std::get<1>(nextAttackLookup);

	if (direction != Direction::NON)
	{
		lookupCoor = m_currentAttackingShip; //TODO check in coliru
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
