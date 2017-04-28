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
	m_player = player;
	m_myBoard.setMembers(board, numRows, numCols);
}
