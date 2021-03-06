#pragma once
#include <memory>

#include "IBattleshipGameAlgo.h"

using char_array = std::unique_ptr<char[]>;
using char_matrix = std::unique_ptr<char_array[]>;
using char_multidim = std::unique_ptr<char_matrix[]>;

class PlayerBoard
{
public:
	PlayerBoard() : m_rows(0), m_cols(0), m_depth(0), m_gameBoard(nullptr) {}

	PlayerBoard(int rows, int cols, int depth);

	PlayerBoard(PlayerBoard&& board) noexcept:
		m_rows(board.m_rows), m_cols(board.m_cols), m_depth(board.m_depth),
		m_gameBoard(std::move(board.m_gameBoard)) {}

	PlayerBoard(const PlayerBoard& board) = delete;
	PlayerBoard& operator=(const PlayerBoard& board) = delete;

	char getBoardSquare(Coordinate coor) const;
	int rows()	const { return m_rows; }
	int cols()	const { return m_cols; }
	int depth() const { return m_depth; }
	void setBoardSquare(Coordinate coor, char sq) const;
	bool coordinateIsValid(Coordinate coor) const;
	void setMembers(int rows, int cols, int depth);

protected:
	int m_rows;
	int m_cols;
	int m_depth;
	char_multidim m_gameBoard;

	void allocateBoard();
};
