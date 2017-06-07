#include "PlayerBoard.h"

PlayerBoard::PlayerBoard(int rows, int cols, int depth) : m_rows(rows), m_cols(cols), m_depth(depth)
{
	m_gameBoard = std::make_unique<char_matrix[]>(m_rows);
	for (auto i = 0; i < m_rows; i++)
	{
		m_gameBoard[i] = std::make_unique<char_array[]>(m_cols);

		for (auto j = 0; j < m_cols; j++)
		{
			m_gameBoard[i][j] = std::make_unique<char[]>(m_depth);
		}
	}

}

char PlayerBoard::getBoardSquare(Coordinate coor) const
{
	if (coordinateIsValid(coor))
		return m_gameBoard[coor.row][coor.col][coor.depth];
	return 0;
}

void PlayerBoard::setBoardSquare(Coordinate coor, char sq) const
{
	if (coordinateIsValid(coor))
		m_gameBoard[coor.row][coor.col][coor.depth] = sq;
}

bool PlayerBoard::coordinateIsValid(Coordinate coor) const
{
	auto r = coor.row, c = coor.col, d = coor.depth;
	return r >= 0 && r < m_rows && c >= 0 && c < m_cols && d >= 0 && d < m_depth;
}

