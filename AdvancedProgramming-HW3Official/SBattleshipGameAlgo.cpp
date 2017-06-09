#include "SBattleshipGameAlgo.h"
#include <algorithm>

// For a cleaner code
using Constant = BattleshipGameUtils::Constants;

void SBattleshipGameAlgo::notifyOnAttackResult(int player, Coordinate coor, AttackResult result)
{
	if (!m_board.coordinateIsValid(coor))
		return;

	switch (result)
	{
	case AttackResult::Miss:
		m_board.setBoardSquare(coor, Constant::MISS_SIGN);
		break;

	case AttackResult::Hit:
		hitNotify(coor, player);
		break;

	case AttackResult::Sink:
		sinkNotify(coor, player);
		break;

	default:
		break;
	}
}

Coordinate SBattleshipGameAlgo::attack()
{
	if (isDone())
		return { -1, -1, -1 };

	if (isRandom())
	{
		auto attackCoor = getNextRandomSquare();

		if (attackCoor.row == -1 && attackCoor.col == -1 && attackCoor.depth == -1)
		{
			m_isDone = true;
			return attackCoor;
		}

		attackCoor.row++;
		attackCoor.col++;
		attackCoor.depth++;

		return attackCoor;
	}

	auto keyForCoordinate = generateKey(m_currentAttackingShip);
	auto& nextAttackDirection = m_attackHelperMap[keyForCoordinate];
	auto row = m_currentAttackingShip.row;
	auto col = m_currentAttackingShip.col;
	auto depth = m_currentAttackingShip.depth;

	if (nextAttackDirection == Direction::NON)
	{
		if ((col < m_board.cols() - 1 && m_board.getBoardSquare({ row, col + 1, depth }) == 'X')
			|| (col > 0 && m_board.getBoardSquare({ row, col - 1, depth }) == 'X'))
		{
			m_lastAttackDirection = Direction::HORIZONTAL;
		}

		else if ((row < m_board.rows() - 1 && m_board.getBoardSquare({ row + 1, col, depth }) == 'X')
			|| (row > 0 && m_board.getBoardSquare({ row - 1, col, depth }) == 'X'))
		{
			m_lastAttackDirection = Direction::VERTICAL;
		}

		else if ((depth < m_board.depth() - 1 && m_board.getBoardSquare({ row, col, depth + 1 }) == 'X')
			|| (depth > 0 && m_board.getBoardSquare({ row, col, depth - 1 }) == 'X'))
		{
			m_lastAttackDirection = Direction::DEPTH;
		}
	}

	return findNextEmptySquare(row, col, depth, nextAttackDirection);
}

void SBattleshipGameAlgo::setBoard(const BoardData & board)
{
	char square;
	// If it's the first time the method is called
	if (!newBorn())
		reset();
	else
		m_newBorn = false;

	auto rows = board.rows(), cols = board.cols(), depth = board.depth();
	m_board.setMembers(rows, cols, depth);
	
	// Sets player board
	for (auto i = 0; i < rows; i++)
	{
		for (auto j = 0; j < cols; j++)
		{
			for (auto k = 0; k < depth; k++)
			{
				Coordinate coor(i, j, k);
				auto sq = board.charAt(coor);
				m_board.setBoardSquare(coor, sq);
			}
		}
	}
	markShipsBoarders();
	std::random_shuffle(m_possibleMoves.begin(), m_possibleMoves.end());
}

void SBattleshipGameAlgo::markShipsBoarders()
{
	// Marks on board all the squares surrounding our ships
	auto rows = m_board.rows(), cols = m_board.cols(), depth = m_board.depth();
	for (auto i = 0; i < rows; i++)
	{
		for (auto j = 0; j < cols; j++)
		{
			for (auto k = 0; k < depth; k++)
			{
				Coordinate coor(i, j, k);
				auto sq = m_board.getBoardSquare(coor);
				if (!isalpha(sq))
				{
					if (i > 0 && isalpha(m_board.getBoardSquare({ i - 1, j, k })))
						m_board.setBoardSquare(coor, Constant::MISS_SIGN);

					else if ((i < rows - 1) && isalpha(m_board.getBoardSquare({ i + 1, j, k })))
						m_board.setBoardSquare(coor, Constant::MISS_SIGN);

					else if (j > 0 && isalpha(m_board.getBoardSquare({ i, j - 1, k })))
						m_board.setBoardSquare(coor, Constant::MISS_SIGN);

					else if ((j < cols - 1) && isalpha(m_board.getBoardSquare({ i, j + 1, k })))
						m_board.setBoardSquare(coor, Constant::MISS_SIGN);

					else if ((j > 0) && isalpha(m_board.getBoardSquare({ i, j, k - 1 })))
						m_board.setBoardSquare(coor, Constant::MISS_SIGN);

					else if ((j < depth - 1) && isalpha(m_board.getBoardSquare({ i, j, k + 1 })))
						m_board.setBoardSquare(coor, Constant::MISS_SIGN);

					else
						m_possibleMoves.push_back(coor);
				}
			}
		}
	}
}

void SBattleshipGameAlgo::reset()
{
	m_currentAttackingShip = Coordinate(-1, -1, -1);
	m_lastAttackDirection = Direction::NON;
	m_randomState = true;
	m_isDone = false;
	m_possibleMoves.clear();
	m_attackHelperMap.clear();
}

void SBattleshipGameAlgo::markOppSankShip(Coordinate coor, bool& changed)
{
	auto sq = m_board.getBoardSquare(coor);
	m_board.setBoardSquare(coor, Constant::MISS_SIGN);
	if (sq != Constant::HIT_SIGN)
	{
		return;
	}
	auto row = coor.row, col = coor.col, depth = coor.depth;
	auto key = generateKey(coor);
	if (m_attackHelperMap.find(key) != m_attackHelperMap.end())
	{
		if (m_currentAttackingShip.row		== row &&
			m_currentAttackingShip.col		== col &&
			m_currentAttackingShip.depth	== depth)
		{
			changed = true;
		}

		m_attackHelperMap.erase(key);
	}

	if (row > 0)
	{
		markRecursiveCall(row - 1, col, depth, changed);
	}

	if (col > 0)
	{
		markRecursiveCall(row, col - 1, depth, changed);
	}

	if (depth > 0)
	{
		markRecursiveCall(row, col, depth - 1, changed);
	}

	if (row < m_board.rows() - 1)
	{
		markRecursiveCall(row + 1, col, depth, changed);
	}

	if (col < m_board.cols() - 1)
	{
		markRecursiveCall(row, col + 1, depth, changed);
	}

	if (depth < m_board.depth() - 1)
	{
		markRecursiveCall(row, col, depth + 1, changed);
	}
}

void SBattleshipGameAlgo::markRecursiveCall(int row, int col, int depth, bool& changed)
{
	Coordinate coor(row, col, depth);
	auto sq = m_board.getBoardSquare(coor);
	if (sq == Constant::HIT_SIGN)
		markOppSankShip(coor, changed);

	else if (sq == Constant::EMPTY_SIGN)
		m_board.setBoardSquare(coor, Constant::MISS_SIGN);
}

void SBattleshipGameAlgo::findNextMove()
{
	if (m_attackHelperMap.empty())
	{
		m_randomState = true;
	}
	else
	{
		auto coordKey = m_attackHelperMap.begin()->first;
		m_currentAttackingShip = retrieveCoordinateFromString(coordKey);
	}
}

void SBattleshipGameAlgo::hitNotify(Coordinate coor, int player)
{
	auto sq = m_board.getBoardSquare(coor);
	m_board.setBoardSquare(coor, Constant::HIT_SIGN);

	if (sq == Constant::EMPTY_SIGN) /* Opponent's ship*/
	{
		if (isRandom() || player != playerID())
		{
			if (isRandom()) {
				m_currentAttackingShip = coor;
				m_randomState = false;
			}

			auto key = generateKey(coor);
			m_attackHelperMap[key] = Direction::NON;
		}

		else
			m_attackHelperMap[generateKey(m_currentAttackingShip)] = m_lastAttackDirection;
	}
}

void SBattleshipGameAlgo::sinkNotify(Coordinate coor, int player)
{
	m_board.setBoardSquare(coor, Constant::HIT_SIGN);
	if (player == playerID())
	{
		auto changed = false;
		markOppSankShip(coor, changed);
		if (changed)
		{
			findNextMove();
		}
	}
}

Coordinate SBattleshipGameAlgo::findNextEmptySquare(int row, int col, int depth, Direction nextAttackDirection)
{
	if (col < m_board.cols() - 1 && shouldAttackSquare(row, col + 1, depth, nextAttackDirection,
														Direction::VERTICAL, Direction::DEPTH))
	{
		m_lastAttackDirection = Direction::HORIZONTAL;
		return { row + 1, col + 2, depth + 1 };
	}

	if (col > 0 && shouldAttackSquare(row, col - 1, depth, nextAttackDirection,
										Direction::VERTICAL, Direction::DEPTH))
	{
		m_lastAttackDirection = Direction::HORIZONTAL;
		return { row + 1, col, depth + 1 };
	}

	if (row < m_board.rows() - 1 && shouldAttackSquare(row + 1, col, depth, nextAttackDirection,
														Direction::HORIZONTAL, Direction::DEPTH))
	{
		m_lastAttackDirection = Direction::VERTICAL;
		return { row + 2, col + 1, depth + 1 };
	}

	if (row > 0 && shouldAttackSquare(row - 1, col, depth, nextAttackDirection,
										Direction::HORIZONTAL, Direction::DEPTH))
	{
		m_lastAttackDirection = Direction::VERTICAL;
		return { row, col + 1, depth + 1 };
	}

	if (depth < m_board.depth() - 1 && shouldAttackSquare(row, col, depth + 1, nextAttackDirection,
														Direction::HORIZONTAL, Direction::VERTICAL))
	{
		m_lastAttackDirection = Direction::DEPTH;
		return { row + 1, col + 1, depth + 2 };
	}

	if (depth > 0 && shouldAttackSquare(row, col, depth - 1, nextAttackDirection,
										Direction::HORIZONTAL, Direction::VERTICAL))
	{
		m_lastAttackDirection = Direction::DEPTH;
		return { row + 1, col + 1, depth };
	}

	return findNextIterative(row, col, depth, nextAttackDirection);
}

bool SBattleshipGameAlgo::shouldAttackSquare(int row, int col, int depth, Direction next,
												Direction otherA, Direction otherB) const
{
	return next != otherA && next != otherB &&
		m_board.getBoardSquare({ row, col, depth }) == Constant::EMPTY_SIGN;
}

Coordinate SBattleshipGameAlgo::findNextIterative(int row, int col, int depth, Direction nextAttackDirection) const
{
	char sq;
	switch (nextAttackDirection)
	{
	case Direction::VERTICAL:
	{
		Coordinate coor(row + 1, col, depth);
		while (coor.row < m_board.rows() &&
			(sq = m_board.getBoardSquare(coor)) != Constant::MISS_SIGN)
		{
			if (sq == Constant::HIT_SIGN)
				++coor.row;

			else if (sq == Constant::EMPTY_SIGN)
			{
				coor.row++, coor.col++, coor.depth++;
				return coor;
			}
		}

		coor.row = row - 1;
		while (coor.row >= 0 && (sq = m_board.getBoardSquare(coor)) != Constant::MISS_SIGN)
		{
			if (sq == Constant::HIT_SIGN)
				--coor.row;

			else if (sq == Constant::EMPTY_SIGN)
			{
				coor.depth++, coor.col++;
				return coor;
			}
		}
		break;
	}

	case Direction::HORIZONTAL:
	{
		Coordinate coor(row, col + 1, depth);
		while (coor.col < m_board.cols() &&
			(sq = m_board.getBoardSquare(coor)) != Constant::MISS_SIGN)
		{
			if (sq == Constant::HIT_SIGN)
				++coor.col;

			else if (sq == Constant::EMPTY_SIGN)
			{
				coor.row++, coor.col++, coor.depth++;
				return coor;
			}
		}

		coor.col = col - 1;
		while (coor.col >= 0 && (sq = m_board.getBoardSquare(coor)) != Constant::MISS_SIGN)
		{
			if (sq == Constant::HIT_SIGN)
				--coor.col;

			else if (sq == Constant::EMPTY_SIGN)
			{
				coor.row++, coor.depth++;
				return coor;
			}
		}
		break;
	}

	case Direction::DEPTH:
	{
		Coordinate coor(row, col, depth + 1);
		while (coor.depth < m_board.depth() &&
			(sq = m_board.getBoardSquare(coor)) != Constant::MISS_SIGN)
		{
			if (sq == Constant::HIT_SIGN)
				++coor.depth;

			else if (sq == Constant::EMPTY_SIGN)
			{
				coor.row++, coor.col++, coor.depth++;
				return coor;
			}
		}

		coor.depth = depth - 1;
		while (coor.depth >= 0 && (sq = m_board.getBoardSquare(coor)) != Constant::MISS_SIGN)
		{
			if (sq == Constant::HIT_SIGN)
				--coor.depth;

			else if (sq == Constant::EMPTY_SIGN)
			{
				coor.row++, coor.col++;
				return coor;
			}
		}
		break;
	}

	default:
		break;
	}
	
	return { -1, -1, -1 };
}

Coordinate SBattleshipGameAlgo::getNextRandomSquare()
{
	while (!m_possibleMoves.empty())
	{
		auto retCoor = m_possibleMoves.back();
		m_possibleMoves.pop_back();
		if (m_board.getBoardSquare(retCoor) == Constant::EMPTY_SIGN)
			return retCoor;
	}
	return { -1, -1, -1 };
}

std::string SBattleshipGameAlgo::generateKey(const Coordinate & coor)
{
	auto ret = std::to_string(coor.row);
	ret += ":";
	ret += std::to_string(coor.col);
	ret += ":";
	ret += std::to_string(coor.depth);
	return ret;
}

Coordinate SBattleshipGameAlgo::retrieveCoordinateFromString(const std::string & str)
{
	std::vector<int> coords;
	if (!BattleshipGameUtils::splitStringByDelimiter(str, coords, ':'))
		return { -1, -1, -1 };
	return { coords[0], coords[1], coords[2] };
}

IBattleshipGameAlgo* GetAlgorithm()
{
	return new SBattleshipGameAlgo();
}
