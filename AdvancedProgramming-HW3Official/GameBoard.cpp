#include "GameBoard.h"
#include <memory>

GameBoard::GameBoard(int rows, int cols, int depth, bool defaultInit) :
		PlayerBoard(rows, cols, depth), m_playerShipCount({0, 0})
{
	if (defaultInit)
	{
		for (auto i = 0; i < m_rows; i++)
		{
			for (auto j = 0; j < m_cols; j++)
			{
				for (auto k = 0; k < m_depth; k++)
				{
					m_gameBoard[i][j][k] = 0;
				}
			}
		}	
	}
}

GameBoard::GameBoard(const GameBoard & board) : 
	GameBoard(board.m_rows, board.m_cols, board.m_depth, false)
{
	m_playerShipCount = board.m_playerShipCount;
	m_boardFileName = board.m_boardFileName;
	
	copyValuesToBoard(board); 
}

GameBoard& GameBoard::operator=(const GameBoard& board)
{
	if (this != &board)
	{
		m_rows = board.rows();
		m_cols = board.cols();
		m_depth = board.depth();
		m_boardFileName = board.name(); 
		m_playerShipCount = board.m_playerShipCount; 

		allocateBoard(); 
		copyValuesToBoard(board); 
	}

	return *this;
}

void GameBoard::copyValuesToBoard(const GameBoard & board) const
{
	if (m_rows != board.rows() || m_cols != board.cols() || m_depth != board.depth())
		return; 

	for (auto i = 0; i < m_rows; i++)
	{
		for (auto j = 0; j < m_cols; j++)
		{
			for (auto k = 0; k < m_depth; k++)
			{
				m_gameBoard[i][j][k] = board.m_gameBoard[i][j][k];
			}
		}
	}
}

