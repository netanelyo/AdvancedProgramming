#include "CommonBattleshipGameAlgo.h"

void CommonBattleshipGameAlgo::Board::setMembers(const char ** board, size_t rows, size_t cols)
{
	m_rows = rows;
	m_cols = cols;
	m_board = new char*[m_rows];
	
	for (auto i = 0; i < m_rows; i++)
	{
		m_board[i] = new char[m_cols];
		
		for (auto j = 0; j < m_cols; j++)
			m_board[i][j] = board[i][j];
	}
}


CommonBattleshipGameAlgo::Board::~Board()
{
	for (auto i = 0; i < m_rows; i++)
		delete[] m_board[i];

	delete[] m_board;
}

void CommonBattleshipGameAlgo::setBoard(int player, const char ** board, int numRows, int numCols)
{
	m_playerID = player;
	char square;

	m_myBoard.setMembers(board, numRows, numCols);

	for (auto i = 0; i < numRows; i++)
	{
		for (auto j = 0; j < numCols; j++)
		{
			square = m_myBoard.getBoardSquare(i, j);
			if (!isalpha(square))
			{
				if (i > 0 && isalpha(m_myBoard.getBoardSquare(i - 1, j)))
					m_myBoard.setBoardSquare(i, j, '#');
				else if (i < 9 && isalpha(m_myBoard.getBoardSquare(i + 1, j)))
					m_myBoard.setBoardSquare(i, j, '#');
				else if (j > 0 && isalpha(m_myBoard.getBoardSquare(i, j - 1)))
					m_myBoard.setBoardSquare(i, j, '#');
				else if (j < 9 && isalpha(m_myBoard.getBoardSquare(i, j + 1)))
					m_myBoard.setBoardSquare(i, j, '#');
			}
		}
	}
}
