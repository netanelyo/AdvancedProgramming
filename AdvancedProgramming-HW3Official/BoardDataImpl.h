#pragma once

#include "GameBoard.h"

class BoardDataImpl : public BoardData
{
public:
	BoardDataImpl(int rows, int cols, int depth, int player) :
		BoardData(), m_board(rows, cols, depth), m_player(player)
	{
		setMembers(rows, cols, depth);
	}
	BoardDataImpl(GameBoard board, int player) : m_board(board), m_player(player)
	{
		setMembers(m_board.rows(), m_board.cols(), m_board.depth());
	}
	char charAt(Coordinate c) const override;

	friend class Game;

private:
	GameBoard m_board;
	int m_player;

	void setPlayer(int player) { m_player = player; }
	void setMembers(int rows, int cols, int depth) { _rows = rows; _cols = cols; _depth = depth; }
};