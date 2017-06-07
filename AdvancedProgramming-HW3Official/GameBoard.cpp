#include "GameBoard.h"
#include <memory>
#include <fstream>

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
	GameBoard(board.m_rows, board.m_cols, board.m_depth)
{
	m_playerShipCount = board.m_playerShipCount;
	m_boardFileName = board.m_boardFileName;
	
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

//Coordinate GameBoard::getNextEmptySquare() const
//{
//	return Coordinate();
//}

void GameBoard::printBoard(std::ofstream& toFile) const
{
	for (auto z = 0; z < m_depth; z++)
	{
		for (auto i = 0; i < m_rows; i++)
		{
			for (auto j = 0; j < m_cols; j++)
			{
				toFile << m_gameBoard[i][j][z];
			}
			toFile << std::endl;
		}
		toFile << std::endl;
	}
}

