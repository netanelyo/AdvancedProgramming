#include "CommonBattleshipGameAlgo.h"
#include <fstream>

void CommonBattleshipGameAlgo::Board::setMembers(const char ** board, size_t rows, size_t cols)
{
	m_rows = rows;
	m_cols = cols;
	m_board = BattleshipUtils::allocateBoard(rows, cols);
	char c;
	
	for (auto i = 0; i < m_rows; i++)
	{
		for (auto j = 0; j < m_cols; j++)
		{
			c = board[i][j];
			m_board[i][j] = isalpha(c) ? c : '0';
		}
	}
}

std::pair<int, int>
CommonBattleshipGameAlgo::Board::getNextEmptySquare(const std::pair<int, int>& pair) const
{
	/*gets next potential col*/
	auto tmp = pair.second;
	auto cond = tmp < (m_cols - 1);
	auto col = cond ? (tmp + 1) : 0;
	/*gets next potential row*/
	tmp = pair.first;
	auto row = cond ? tmp : tmp + 1;

	bool valid;

	/*finds the first empty square*/
	while ((valid = validateCoordinate(row, col)) == true && getBoardSquare(row, col) != '0')
	{
		if (col == (getBoardCols() - 1))
		{
			col = 0;
			row++;
		}
		else
			col++;
	}

	if (!valid)
	{
		return std::pair<int, int>(-1, -1);
	}

	return std::pair<int, int>(row, col);
}

CommonBattleshipGameAlgo::Board::~Board()
{
	BattleshipUtils::deallocateBoard(m_board, m_rows);
}

void CommonBattleshipGameAlgo::setBoard(int player, const char ** board, int numRows, int numCols)
{
	m_playerID = player;
	char square;
	/*sets our board*/
	m_myBoard.setMembers(board, numRows, numCols);

	/*marks on board all the squares surrounding our ships*/
	for (auto i = 0; i < numRows; i++)
	{
		for (auto j = 0; j < numCols; j++)
		{
			square = m_myBoard.getBoardSquare(i, j);
			if (!isalpha(square))
			{
				if (i > 0 && isalpha(m_myBoard.getBoardSquare(i - 1, j)))
					m_myBoard.setBoardSquare(i, j, '#');
				else if ((i < numRows - 1) && isalpha(m_myBoard.getBoardSquare(i + 1, j)))
					m_myBoard.setBoardSquare(i, j, '#');
				else if (j > 0 && isalpha(m_myBoard.getBoardSquare(i, j - 1)))
					m_myBoard.setBoardSquare(i, j, '#');
				else if ((j < numCols - 1) && isalpha(m_myBoard.getBoardSquare(i, j + 1)))
					m_myBoard.setBoardSquare(i, j, '#');
			}
		}
	}
}

void CommonBattleshipGameAlgo::notifyOnAttackResult(int player, int row, int col, AttackResult result)
{
	row--;
	col--;
	if (!m_myBoard.validateCoordinate(row, col))
		return;

	if (m_myBoard.getBoardSquare(row, col) != '0' && m_myBoard.getBoardSquare(row, col) != 'X')
	{
		switch (result)
		{
		case AttackResult::Sink:
		case AttackResult::Hit:
			m_myBoard.setBoardSquare(row, col, 'X');
			break;
		default:
			break;
		}
	}
}

std::pair<int, int> CommonBattleshipGameAlgo::attack()
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

	m_currentSeqMove.first = row++;
	m_currentSeqMove.second = col++;

	return attackPair;
}

