#include "GameBoard.h"
#include <memory>
#include <iostream>

GameBoard::GameBoard(int rows, int cols, int depth, bool defaultInit) :
		m_rows(rows), m_cols(cols), m_depth(depth), m_playerShipCount({0, 0})
{
	m_gameBoard = std::make_unique<char_matrix[]>(m_rows);
	for (auto i = 0; i < m_rows; i++)
	{
		m_gameBoard[i] = std::make_unique<char_array[]>(m_cols);

		for (auto j = 0; j < m_cols; j++)
		{
			m_gameBoard[i][j] = std::make_unique<char[]>(m_depth);
			
			if (defaultInit)
			{
				for (auto k = 0; k < m_depth; k++)
					m_gameBoard[i][j][k] = 0;
			}
		}
	}
}

GameBoard::GameBoard(GameBoard && board) noexcept
{
	m_rows		= board.m_rows;
	m_cols		= board.m_cols;
	m_depth		= board.m_depth;
	m_gameBoard = std::move(board.m_gameBoard);
}

char GameBoard::getBoardSquare(Coordinate coor) const
{
	if (coordinateIsValid(coor))
		return m_gameBoard[coor.row][coor.col][coor.depth];
	return 0;
}

//Coordinate GameBoard::getNextEmptySquare() const
//{
//	return Coordinate();
//}

void GameBoard::setBoardSquare(Coordinate coor, char sq) const
{
	if (coordinateIsValid(coor))
		m_gameBoard[coor.row][coor.col][coor.depth] = sq;
}

bool GameBoard::coordinateIsValid(Coordinate coor) const
{
	auto r = coor.row, c = coor.col, d = coor.depth;
	return r >= 0 && r < m_rows && c >= 0 && c < m_cols && d >= 0 && d < m_depth;
}

void GameBoard::printBoard()
{
	for (auto z = 0; z < m_depth; z++)
	{
		for (auto i = 0; i < m_rows; i++)
		{
			for (auto j = 0; j < m_cols; j++)
			{
				std::cout << m_gameBoard[i][j][z];
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

