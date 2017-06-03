#pragma once

#include "GameBoard.h"
#include "Game.h"

class BoardDataImpl : public BoardData
{
public:
	BoardDataImpl(int rows, int cols, int depth, int player) :
		BoardData(), m_gameBoard(rows, cols, depth), m_player(player) {}
	char charAt(Coordinate c) const override;

	friend class Game;

private:
	GameBoard m_gameBoard;
	int m_player;

	void setPlayer(int player) { m_player = player; }
};