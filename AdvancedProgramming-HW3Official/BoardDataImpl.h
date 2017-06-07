#pragma once

#include "PlayerBoard.h"
#include "Game.h"

class BoardDataImpl : public BoardData
{
public:
	BoardDataImpl(int rows, int cols, int depth, int player) :
		BoardData(), m_board(rows, cols, depth), m_player(player) {}
	BoardDataImpl(GameBoard board, int player) : m_board(board), m_player(player) {}
	char charAt(Coordinate c) const override;

	friend class Game;

private:
	GameBoard m_board;
	int m_player;

	void setPlayer(int player) { m_player = player; }
};