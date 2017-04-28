#include "CommonBattleshipGameAlgo.h"

void CommonBattleshipGameAlgo::Board::setMembers(const char ** board, size_t rows, size_t cols)
{
	m_rows = rows;
	m_cols = cols;
	m_board = BattleshipUtils::allocateBoard(rows, cols);
	
	for (auto i = 0; i < m_rows; i++)
	{
		for (auto j = 0; j < m_cols; j++)
			m_board[i][j] = board[i][j];
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

	std::pair<int, int> retPair;
	/*finds the first empty square*/
	while (getBoardSquare(row, col) != '0')
	{
		if (col == (getBoardCols() - 1))
		{
			col = 0;
			row++;
		}
		else
			col++;
	}

	return retPair;
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
	if (m_myBoard.getBoardSquare(row, col) != '0' && m_myBoard.getBoardSquare(row, col) != 'X')
	{
		switch (result)
		{
		case AttackResult::Sink:
			m_shipCounter--;
		case AttackResult::Hit:
			m_myBoard.setBoardSquare(row, col, 'X');
			break;
		default:
			break;
		}
	}
}